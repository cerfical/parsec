#include "src_gen/LexerSpec.hpp"

#include "regex/ast.hpp"
#include <algorithm>

using namespace parsec::regex::ast;

namespace parsec::src_gen {
	namespace {
		class ConvertRegexToRule : private regex::ast::NodeVisitor {
		public:

			fg::Rule operator()(const regex::RegularExpr& regex) {
				regex.rootNode()->acceptVisitor(*this);
				return std::move(m_rule);
			}

		private:
			void visit(const CharAtom& n) override {
				m_rule = fg::Rule(std::string(1, n.value));
			}

			void visit(const NilExpr& n) override {
				m_rule = {};
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
				auto right = visitBinary(n);
				m_rule.altern(std::move(right));
			}

			void visit(const ConcatExpr& n) override {
				auto right = visitBinary(n);
				m_rule.concat(std::move(right));
			}


			fg::Rule visitBinary(const BinaryExpr& n) {
				n.right->acceptVisitor(*this);
				fg::Rule right = std::move(m_rule);

				n.left->acceptVisitor(*this);
				return right;
			}


			parsec::fg::Rule m_rule;
		};
	}


	void LexerSpec::defineToken(std::string_view name, const regex::RegularExpr& pattern) {
		m_tokens.emplace_back();
		try {
			m_inputSyntax.insertSymbol(name, ConvertRegexToRule()(pattern));
		} catch(...) {
			m_tokens.pop_back();
			throw;
		}
		m_tokens.back() = m_inputSyntax.symbols().back();
		m_tokensSorted = false;
	}


	bool LexerSpec::isToken(std::string_view name) const {
		if(std::ranges::binary_search(m_tokens, name)) {
			return true;
		}
		return false;
	}


	std::span<const std::string_view> LexerSpec::tokens() const {
		if(!m_tokensSorted) {
			std::ranges::sort(m_tokens);
			m_tokensSorted = true;
		}
		return m_tokens;
	}
}