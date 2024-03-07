#include "src_gen/LexerSpec.hpp"
#include "regex/ast.hpp"

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
			void visit(const CharAtom& n) {
				m_rule = fg::Rule(std::string(1, n.value));
			}

			void visit(const NilExpr& n) {
				m_rule = {};
			}

			void visit(const PlusExpr& n) {
				m_rule.repeatPlus();
			}

			void visit(const StarExpr& n) {
				m_rule.repeatStar();
			}

			void visit(const OptionalExpr& n) {
				m_rule.markOptional();
			}

			void visit(const AlternExpr& n) {
				auto right = visitBinary(n);
				m_rule.altern(std::move(right));
			}

			void visit(const ConcatExpr& n) {
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
		m_tokens.insertSymbol(name, ConvertRegexToRule()(pattern));
	}
}