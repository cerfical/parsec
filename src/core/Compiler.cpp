#include "core/Compiler.hpp"

#include "regex/RegularExpr.hpp"
#include "regex/ast.hpp"

#include "pars/Parser.hpp"
#include "utils/string_utils.hpp"
#include "pars/ast.hpp"
#include "src_gen.hpp"

#include <sstream>
#include <format>

using namespace parsec::pars::ast;
using namespace parsec::regex::ast;

namespace parsec {
	namespace {
		constexpr auto eofTokenName = "Eof", unnamedTokenPrefix = "Unnamed";


		
		std::string unifyName(std::string_view name) {
			return string_utils::toPascalCase(name);
		}

		std::string makeName(const pars::Token& tok) {
			auto name = unifyName(tok.text());
			if(name.empty()) {
				throw std::runtime_error("name must contain at least one letter");
			}
			return name;
		}

		fg::Rule compileRegex(const ExprNode& n) {
			class : private regex::ast::NodeVisitor {
			public:
				
				fg::Rule operator()(const ExprNode& n) {
					n.acceptVisitor(*this);
					return std::move(m_rule);
				}

			private:
				void visit(const CharAtom& n) override {
					m_rule = fg::Rule(std::string(1, n.value));
				}

				void visit(const NilExpr& n) override {
					m_rule = fg::Rule();
				}

				void visit(const PlusExpr& n) override {
					n.inner->acceptVisitor(*this);
					m_rule.repeatPlus();
				}

				void visit(const StarExpr& n) override {
					n.inner->acceptVisitor(*this);
					m_rule.repeatStar();
				}

				void visit(const OptionalExpr& n) override {
					n.inner->acceptVisitor(*this);
					m_rule.markOptional();
				}

				void visit(const AlternExpr& n) override {
					m_rule = compileRegex(*n.left);
					m_rule.altern(compileRegex(*n.right));
				}

				void visit(const ConcatExpr& n) override {
					m_rule = compileRegex(*n.left);
					m_rule.concat(compileRegex(*n.right));
				}


				fg::Rule m_rule;
			} impl;

			return impl(n);
		}



		class PatternCache : private pars::ast::NodeVisitor {
		public:

			PatternCache(const Node& n) {
				m_grammar.insertSymbol(unifyName(eofTokenName));
				n.acceptVisitor(*this);
			}


			const std::string& getPatternName(std::string_view pattern) {
				const auto [patternNameIt, wasInserted] = m_patternNames.try_emplace(pattern);
				auto& patternName = patternNameIt->second;

				if(wasInserted) {
					patternName = unifyName(std::format("{}{}", unnamedTokenPrefix, m_inlinePatternId++));
					cacheNamedPattern(patternName, pattern);
				}
				return patternName;
			}

			const fg::SymbolGrammar& grammar() const noexcept {
				return m_grammar;
			}


		private:
			void visit(const NodeList& n) {
				n.rest->acceptVisitor(*this);
				n.last->acceptVisitor(*this);
			}

			void visit(const NamedPattern& n) {
				const auto patternName = makeName(n.name);
				m_patternNames[n.pattern.text()] = patternName;
				cacheNamedPattern(patternName, n.pattern.text());
			}
			
			void cacheNamedPattern(const std::string& name, std::string_view pattern) {
				const auto regex = regex::RegularExpr(pattern);
				m_grammar.insertSymbol(name, compileRegex(*regex.rootNode()));
			}
			

			void visit(const InlinePattern& n) {}
			void visit(const NilNode& n) {}
			void visit(const NilRule& n) {}
			void visit(const NameRule& n) {}
			void visit(const ConcatRule& n) {}
			void visit(const AlternRule& n) {}
			void visit(const OptionalRule& n) {}
			void visit(const PlusRule& n) {}
			void visit(const StarRule& n) {}
			void visit(const NamedRule& n) {}


			fg::SymbolGrammar m_grammar;
			std::unordered_map<std::string_view, std::string> m_patternNames;
			std::size_t m_inlinePatternId = 0;
		};



		class CompiledSpec : private pars::ast::NodeVisitor {
		public:

			CompiledSpec(const Node& ast)
				: m_patterns(ast) {
				ast.acceptVisitor(*this);
			}


			const fg::SymbolGrammar& parser() const noexcept {
				return m_parser;
			}

			const fg::SymbolGrammar& lexer() const noexcept {
				return m_patterns.grammar();
			}


		private:
			void visit(const NilNode& n) {}

			void visit(const NodeList& n) {
				n.rest->acceptVisitor(*this);
				n.last->acceptVisitor(*this);
			}



			void visit(const InlinePattern& n) {
				const auto& inlinePatternName = m_patterns.getPatternName(n.pattern.text());
				m_rule = fg::Rule(inlinePatternName);
			}

			void visit(const NamedPattern& n) {}



			void visit(const NilRule& n) {
				m_rule = fg::Rule();
			}

			void visit(const NameRule& n) {
				m_rule = fg::Rule(makeName(n.name));
			}

			void visit(const ConcatRule& n) {
				n.right->acceptVisitor(*this);
				fg::Rule right = std::move(m_rule);
				n.left->acceptVisitor(*this);
				m_rule.concat(std::move(right));
			}

			void visit(const AlternRule& n) {
				n.right->acceptVisitor(*this);
				fg::Rule right = std::move(m_rule);
				n.left->acceptVisitor(*this);
				m_rule.altern(std::move(right));
			}

			void visit(const OptionalRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule.markOptional();
			}

			void visit(const PlusRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule.repeatPlus();
			}
			
			void visit(const StarRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule.repeatStar();
			}

			void visit(const NamedRule& n) {
				n.rule->acceptVisitor(*this);

				const auto ruleName = makeName(n.name);
				m_parser.insertSymbol(ruleName, std::move(m_rule));

				// make the first symbol encountered the start symbol
				if(m_parser.startSymbol().empty()) {
					m_parser.setStartSymbol(ruleName);
				}
			}



			fg::SymbolGrammar m_parser;
			PatternCache m_patterns;
			fg::Rule m_rule;
		};
	}


	void Compiler::compile(std::istream& input) {
		const auto ast = pars::Parser().parse(input);
		CompiledSpec spec(*ast.get());
		
		src_gen::ConfigStore configs;
		configs.eofTokenName(unifyName(eofTokenName));

		src_gen::CppLexerGen(*m_output)
			.run(spec.lexer(), configs);
		
		src_gen::CppParserGen(*m_output)
			.run(spec.parser(), configs);
	}


	void Compiler::compile(std::string_view str) {
		auto input = std::istringstream(std::string(str));
		compile(input);
	}
}