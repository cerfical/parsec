#include "core/Compiler.hpp"

#include "core/RuleConflictError.hpp"
#include "core/ParseError.hpp"

#include "utils/string_utils.hpp"
#include "src_gen/CppCodeGen.hpp"
#include "regex/make.hpp"
#include "pars.hpp"

#include <sstream>
#include <format>

namespace parsec {
	using namespace pars;
	using namespace src_gen;
	
	namespace {
		constexpr auto eofTokenName = "Eof", unnamedTokenPrefix = "Unnamed";
		
		Symbol makeName(const Token& name) {
			return string_utils::toPascalCase(name.text());
		}


		class AstTraverser : private NodeVisitor {
		public:

			void traverse(const Node& ast) {
				ast.acceptVisitor(*this);
			}

		private:
			void visit(const NamedRule& n) override {
				n.rule()->acceptVisitor(*this);
			}

			void visit(const ConcatRule& n) override {
				n.left()->acceptVisitor(*this);
				n.right()->acceptVisitor(*this);
			}

			void visit(const AlternRule& n) override {
				n.left()->acceptVisitor(*this);
				n.right()->acceptVisitor(*this);
			}

			void visit(const OptionalRule& n) override {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const PlusRule& n) override {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const StarRule& n) override {
				n.inner()->acceptVisitor(*this);
			}

			void visit(const ListNode& n) override {
				n.head()->acceptVisitor(*this);
				n.tail()->acceptVisitor(*this);
			}

			void visit(const SymbolAtom& n) override {}
			void visit(const InlineToken& n) override {}
			void visit(const EmptyRule& n) override {}
			void visit(const NamedToken& n) override {}
			void visit(const EmptyNode& n) override {}
		};


		class PatternNameCache {
		public:

			void insertEntry(const std::string& pattern, const Symbol& name) {
				m_patternsToNames[pattern] = name;
			}

			const Symbol* lookupName(const std::string& pattern) const {
				const auto patternToNameIt = m_patternsToNames.find(pattern);
				if(patternToNameIt != m_patternsToNames.end()) {
					return &patternToNameIt->second;
				}
				return nullptr;
			}

		private:
			std::unordered_map<std::string, Symbol> m_patternsToNames;
		};


		class NameTable {
		public:

			void insertEntry(const Symbol& name, const Token& tok) {
				m_namesToTokens[name] = tok;
			}

			const Token* lookupToken(const Symbol& name) const {
				const auto nameToTokenIt = m_namesToTokens.find(name);
				if(nameToTokenIt != m_namesToTokens.end()) {
					return &nameToTokenIt->second;
				}
				return nullptr;
			}

			bool contains(const Symbol& name) const {
				return m_namesToTokens.contains(name) || name == eofTokenName;
			}

		private:
			struct SymbolHasher {
				std::size_t operator()(const Symbol& symbol) const noexcept {
					return hash(symbol);
				}
			};

			std::unordered_map<Symbol, Token, SymbolHasher> m_namesToTokens;
		};


		void collectNamesAndPatterns(const Node& ast, NameTable& names, PatternNameCache& patterns) {
			class Impl : public AstTraverser {
			public:

				Impl(NameTable& names, PatternNameCache& patterns)
					: m_names(names), m_patterns(patterns) {}

			private:
				void visit(const NamedToken& n) override {
					insertName(n.name());
					m_patterns.insertEntry(n.pattern().text(), makeName(n.name()));
				}

				void visit(const NamedRule& n) override {
					insertName(n.name());
					// no need to visit the rule itself
				}

				void insertName(const Token& name) {
					const auto unifiedName = makeName(name);
					if(!unifiedName) {
						throw ParseError::emptyName(name.loc());
					}

					if(isReservedName(unifiedName)) {
						throw ParseError::reservedNameRedefine(name.loc());
					}

					if(m_names.contains(unifiedName)) {
						throw ParseError::nameRedefine(name.loc());
					}
					m_names.insertEntry(unifiedName, name);
				}

				static bool isReservedName(const Symbol& name) {
					if(name.value().starts_with(unnamedTokenPrefix) || name == eofTokenName) {
						return true;
					}
					return false;
				}

				PatternNameCache& m_patterns;
				NameTable& m_names;
			} impl(names, patterns);
			impl.traverse(ast);
		}


		void checkForUndefinedNames(const Node& ast, const NameTable& names) {
			class Impl : public AstTraverser {
			public:

				Impl(const NameTable& names)
					: m_names(names) {}

			private:
				void visit(const SymbolAtom& n) override {
					if(!m_names.contains(makeName(n.value()))) {
						throw ParseError::undefinedName(n.value().loc());
					}
				}

				const NameTable& m_names;
			} impl(names);
			impl.traverse(ast);
		}


		SymbolGrammar compileTokenGrammar(const Node& ast, NameTable& names, PatternNameCache& patterns) {
			class Impl : private AstTraverser {
			public:

				Impl(NameTable& names, PatternNameCache& patterns)
					: m_names(names), m_patterns(patterns) {}

				SymbolGrammar operator()(const Node& ast) {
					traverse(ast);
					return std::move(m_tokens);
				}

			private:
				void visit(const NamedToken& n) override {
					defineToken(makeName(n.name()), n.pattern());
				}

				void visit(const InlineToken& n) override {
					const auto namePtr = m_patterns.lookupName(n.pattern().text());
					if(!namePtr) {
						const auto fakeName = std::format("{}{}", unnamedTokenPrefix, m_nextUnnamedTokenId++);
						m_patterns.insertEntry(n.pattern().text(), fakeName);
						m_names.insertEntry(fakeName, n.pattern());
						
						defineToken(fakeName, n.pattern());
					} else if(!m_tokens.contains(*namePtr)) {
						defineToken(*namePtr, n.pattern());
					}
				}

				void defineToken(const Symbol& name, const Token& pattern) {
					RegularExpr regex;
					try {
						regex = RegularExpr(pattern.text());
					} catch(const ParseError& e) {
						// adjust the error location to take into account the location of the pattern token
						throw ParseError(
							SourceLoc(
								pattern.loc().startCol() + e.loc().startCol() + 1,
								e.loc().colCount(),
								pattern.loc().lineNo(),
								pattern.loc().linePos()
							),
							e.what()
						);
					}
					m_tokens.define(name, regex);
				}
				
				SymbolGrammar m_tokens;
				std::size_t m_nextUnnamedTokenId = 0;
				
				NameTable& m_names;
				PatternNameCache& m_patterns;
			} impl(names, patterns);
			return impl(ast);
		}


		SymbolGrammar compileRuleGrammar(const Node& ast, const NameTable& names, const PatternNameCache& patterns) {
			class Impl : private AstTraverser {
			public:

				Impl(const NameTable& names, const PatternNameCache& patterns)
					: m_names(names), m_patterns(patterns) {}

				SymbolGrammar operator()(const Node& ast) {
					traverse(ast);
					return std::move(m_rules);
				}

			private:
				void visit(const NamedRule& n) override {
					traverse(*n.rule());

					const auto unifedName = makeName(n.name());
					m_rules.define(unifedName, m_rule);

					// make the first symbol encountered the start symbol
					if(!m_rules.root()) {
						m_rules.setRoot(unifedName);
					}
				}

				void visit(const InlineToken& n) override {
					m_rule = regex::atom(*m_patterns.lookupName(n.pattern().text()));
				}

				void visit(const EmptyRule& n) override {
					m_rule = regex::empty();
				}

				void visit(const SymbolAtom& n) override {
					m_rule = regex::atom(makeName(n.value()));
				}

				void visit(const ConcatRule& n) override {
					const regex::NodePtr left = (traverse(*n.left()), m_rule);
					m_rule = (traverse(*n.right()), regex::concat(left, m_rule));
				}

				void visit(const AlternRule& n) override {
					const regex::NodePtr left = (traverse(*n.left()), m_rule);
					m_rule = (traverse(*n.right()), regex::altern(left, m_rule));
				}

				void visit(const OptionalRule& n) override {
					traverse(*n.inner());
					m_rule = regex::optional(m_rule);
				}

				void visit(const PlusRule& n) override {
					traverse(*n.inner());
					m_rule = regex::plusClosure(m_rule);
				}

				void visit(const StarRule& n) override {
					traverse(*n.inner());
					m_rule = regex::starClosure(m_rule);
				}


				SymbolGrammar m_rules;
				regex::NodePtr m_rule;

				const NameTable& m_names;
				const PatternNameCache& m_patterns;
			} impl(names, patterns);
			return impl(ast);
		}
	}


	void Compiler::compile(std::istream& input) {
		const auto ast = Parser().parse(input);
		
		NameTable names;
		PatternNameCache patterns;

		collectNamesAndPatterns(*ast, names, patterns);
		checkForUndefinedNames(*ast, names);

		auto tokens = compileTokenGrammar(*ast, names, patterns);
		tokens.define(eofTokenName, {});

		auto rules = compileRuleGrammar(*ast, names, patterns);

		try {
			CppCodeGen(*m_output).run(
				tokens,
				rules,
				ConfigStore()
					.eofTokenName(eofTokenName)
			);
		} catch(const RuleConflictError& err) {
			const auto srcTokPtr1 = names.lookupToken(err.rule1().value());
			const auto srcTokPtr2 = names.lookupToken(err.rule2().value());

			if(tokens.contains(err.rule1())) {
				throw ParseError::patternConflict(srcTokPtr1->loc(), srcTokPtr2->text());
			} else {
				throw ParseError::ruleConflict(srcTokPtr1->loc(), srcTokPtr2->text());
			}
		}
	}


	void Compiler::compile(std::string_view str) {
		auto input = std::istringstream(std::string(str));
		compile(input);
	}
}