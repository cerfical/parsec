#include "stringify.hpp"

#include "core/RegularExpr.hpp"

#include "pars/TokenKinds.hpp"
#include "pars/Token.hpp"

#include "regex/NodeVisitor.hpp"
#include "regex/nodes.hpp"

#include <sstream>
#include <format>

namespace parsec {
	std::string stringify(const RegularExpr& expr) {
		return expr ? stringify(*static_cast<regex::NodePtr>(expr)) : "()";
	}


	std::string stringify(pars::TokenKinds tok) {
		switch(tok) {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) case pars::TokenKinds::tok: return #tok;
			
			PARSEC_PARS_TOKEN_KIND_LIST

#undef PARSEC_PARS_TOKEN_KIND

		}
		return "UnknownTokenType";
	}


	std::string stringify(const pars::Token& tok) {
		return std::format("({}: \"{}\")", stringify(tok.kind()), tok.text());
	}


	std::string stringify(const regex::ExprNode& n) {
		using namespace regex;

		class StringifyImpl : private NodeVisitor {
		public:

			std::string run(const ExprNode& n) {
				n.accept(*this);
				return std::move(m_out).str();
			}

		private:
			void visit(const SymbolAtom& n) override {
				m_out << n.symbol();
			}

			void visit(const OptionalExpr& n) override {
				n.inner()->accept(*this);
				m_out << '?';
			}

			void visit(const PlusClosure& n) override {
				n.inner()->accept(*this);
				m_out << '+';
			}

			void visit(const StarClosure& n) override {
				n.inner()->accept(*this);
				m_out << '*';
			}

			void visit(const AlternExpr& n) override {
				m_out << '(';
				n.left()->accept(*this);
				m_out << " | ";
				n.right()->accept(*this);
				m_out << ')';
			}

			void visit(const ConcatExpr& n) override {
				m_out << '(';
				n.left()->accept(*this);
				m_out << ' ';
				n.right()->accept(*this);
				m_out << ')';
			}

			std::ostringstream m_out;
		};

		return StringifyImpl().run(n);
	}
}