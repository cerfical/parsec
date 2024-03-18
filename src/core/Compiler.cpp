#include "core/Compiler.hpp"

#include "pars/Parser.hpp"
#include "utils/string_utils.hpp"
#include "pars/ast.hpp"
#include "src_gen.hpp"

#include <sstream>
#include <format>

using namespace parsec::pars::ast;

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



		class PatternCache : private pars::ast::NodeVisitor {
		public:

			PatternCache(const Node& n) {
				m_grammar.define(unifyName(eofTokenName), {});
				n.acceptVisitor(*this);
			}


			const std::string& getTokenName(const pars::Token& pattern) {
				const auto [stringToNameIt, wasInserted] = pattern.is(pars::TokenKinds::PatternString) ?
					m_patternStringNames.try_emplace(pattern.text()) :
					m_rawStringNames.try_emplace(pattern.text());

				if(wasInserted) {
					stringToNameIt->second = unifyName(std::format("{}{}", unnamedTokenPrefix, m_inlinePatternId++));
					defineToken(stringToNameIt->second, pattern);
				}

				return stringToNameIt->second;
			}

			const fg::SymbolGrammar& grammar() const noexcept {
				return m_grammar;
			}


		private:
			void visit(const NodeList& n) {
				n.rest->acceptVisitor(*this);
				n.last->acceptVisitor(*this);
			}

			void visit(const NamedToken& n) {
				const auto patternName = makeName(n.name);
				if(n.pattern.is(pars::TokenKinds::PatternString)) {
					m_patternStringNames[n.pattern.text()] = patternName;
				} else {
					m_rawStringNames[n.pattern.text()] = patternName;
				}
				defineToken(patternName, n.pattern);
			}
			
			void defineToken(const std::string& name, const pars::Token& pattern) {
				if(pattern.is(pars::TokenKinds::PatternString)) {
					m_grammar.define(name, fg::RegularExpr::fromPatternString(pattern.text()));
				} else {
					m_grammar.define(name, fg::RegularExpr::fromRawString(pattern.text()));
				}
			}
			

			void visit(const InlineToken& n) {}
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
			std::unordered_map<std::string_view, std::string> m_patternStringNames;
			std::unordered_map<std::string_view, std::string> m_rawStringNames;
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



			void visit(const InlineToken& n) {
				const auto& inlinePatternName = m_patterns.getTokenName(n.pattern);
				m_rule = fg::Symbol(inlinePatternName);
			}

			void visit(const NamedToken& n) {}



			void visit(const NilRule& n) {
				m_rule = fg::Symbol();
			}

			void visit(const NameRule& n) {
				m_rule = fg::Symbol(makeName(n.name));
			}

			void visit(const ConcatRule& n) {
				n.left->acceptVisitor(*this);
				auto left = m_rule;
				n.right->acceptVisitor(*this);
				m_rule = left + m_rule;
			}

			void visit(const AlternRule& n) {
				n.left->acceptVisitor(*this);
				auto left = m_rule;
				n.right->acceptVisitor(*this);
				m_rule = left | m_rule;
			}

			void visit(const OptionalRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule = optional(m_rule);
			}

			void visit(const PlusRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule = plusClosure(m_rule);
			}
			
			void visit(const StarRule& n) {
				n.inner->acceptVisitor(*this);
				m_rule = starClosure(m_rule);
			}

			void visit(const NamedRule& n) {
				n.rule->acceptVisitor(*this);

				const auto ruleName = makeName(n.name);
				m_parser.define(ruleName, m_rule);

				// make the first symbol encountered the start symbol
				if(!m_parser.root()) {
					m_parser.setRoot(ruleName);
				}
			}



			fg::SymbolGrammar m_parser;
			PatternCache m_patterns;
			fg::RegularExpr m_rule;
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