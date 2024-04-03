#include "core/Compiler.hpp"

#include "pars/Parser.hpp"
#include "utils/string_utils.hpp"
#include "pars/ast.hpp"
#include "src_gen/CppCodeGen.hpp"
#include "regex/make.hpp"

#include <sstream>
#include <format>

using namespace parsec::pars::ast;
using namespace parsec::pars;

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
				const auto [stringToNameIt, wasInserted] = pattern.is<pars::TokenKinds::PatternString>() ?
					m_patternStringNames.try_emplace(pattern.text()) :
					m_rawStringNames.try_emplace(pattern.text());

				if(wasInserted) {
					stringToNameIt->second = unifyName(std::format("{}{}", unnamedTokenPrefix, m_inlinePatternId++));
					defineToken(stringToNameIt->second, pattern);
				}

				return stringToNameIt->second;
			}

			const SymbolGrammar& grammar() const noexcept {
				return m_grammar;
			}


		private:
			void visit(const ListNode& n) {
				n.head()->acceptVisitor(*this);
				n.tail()->acceptVisitor(*this);
			}

			void visit(const NamedToken& n) {
				const auto patternName = makeName(n.name());
				if(n.pattern().is<pars::TokenKinds::PatternString>()) {
					m_patternStringNames[n.pattern().text()] = patternName;
				} else {
					m_rawStringNames[n.pattern().text()] = patternName;
				}
				defineToken(patternName, n.pattern());
			}
			
			void defineToken(const std::string& name, const pars::Token& pattern) {
				if(pattern.is<pars::TokenKinds::PatternString>()) {
					m_grammar.define(name, RegularExpr::fromPatternString(pattern.text()));
				} else {
					m_grammar.define(name, RegularExpr::fromRawString(pattern.text()));
				}
			}
			

			void visit(const InlineToken& n) {}
			void visit(const EmptyNode& n) {}
			void visit(const EmptyRule& n) {}
			void visit(const ast::SymbolRule& n) {}
			void visit(const ConcatRule& n) {}
			void visit(const AlternRule& n) {}
			void visit(const OptionalRule& n) {}
			void visit(const PlusRule& n) {}
			void visit(const StarRule& n) {}
			void visit(const NamedRule& n) {}


			SymbolGrammar m_grammar;
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


			const SymbolGrammar& parser() const noexcept {
				return m_parser;
			}

			const SymbolGrammar& lexer() const noexcept {
				return m_patterns.grammar();
			}


		private:
			void visit(const EmptyNode& n) {}

			void visit(const ListNode& n) {
				n.head()->acceptVisitor(*this);
				n.tail()->acceptVisitor(*this);
			}



			void visit(const InlineToken& n) {
				const auto& inlinePatternName = m_patterns.getTokenName(n.pattern());
				m_rule = regex::atom(inlinePatternName);
			}

			void visit(const NamedToken& n) {}



			void visit(const EmptyRule& n) {
				m_rule = regex::empty();
			}

			void visit(const ast::SymbolRule& n) {
				m_rule = regex::atom(makeName(n.symbol()));
			}

			void visit(const ConcatRule& n) {
				n.left()->acceptVisitor(*this);
				auto left = m_rule;
				n.right()->acceptVisitor(*this);
				m_rule = left + m_rule;
			}

			void visit(const AlternRule& n) {
				n.left()->acceptVisitor(*this);
				auto left = m_rule;
				n.right()->acceptVisitor(*this);
				m_rule = left | m_rule;
			}

			void visit(const OptionalRule& n) {
				n.inner()->acceptVisitor(*this);
				m_rule = regex::optional(m_rule);
			}

			void visit(const PlusRule& n) {
				n.inner()->acceptVisitor(*this);
				m_rule = regex::plusClosure(m_rule);
			}
			
			void visit(const StarRule& n) {
				n.inner()->acceptVisitor(*this);
				m_rule = regex::starClosure(m_rule);
			}

			void visit(const NamedRule& n) {
				n.rule()->acceptVisitor(*this);

				const auto ruleName = makeName(n.name());
				m_parser.define(ruleName, m_rule);

				// make the first symbol encountered the start symbol
				if(!m_parser.root()) {
					m_parser.setRoot(ruleName);
				}
			}



			SymbolGrammar m_parser;
			PatternCache m_patterns;
			RegularExpr m_rule;
		};
	}


	void Compiler::compile(std::istream& input) {
		const auto ast = pars::Parser().parse(input);
		CompiledSpec spec(*ast.get());
		
		src_gen::ConfigStore configs;
		configs.eofTokenName(unifyName(eofTokenName));

		src_gen::CppCodeGen(*m_output)
			.run(spec.lexer(), spec.parser(), configs);
	}


	void Compiler::compile(std::string_view str) {
		auto input = std::istringstream(std::string(str));
		compile(input);
	}
}