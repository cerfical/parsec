#include "print.hpp"

#include "core/SourceLoc.hpp"
#include "core/SymbolRule.hpp"
#include "core/RegularExpr.hpp"
#include "core/SymbolGrammar.hpp"
#include "core/Symbol.hpp"

#include "pars/TokenKinds.hpp"
#include "pars/Token.hpp"

#include "regex/nodes.hpp"

#include "elr.hpp"
#include "dfa.hpp"

namespace parsec {
	void print(const elr::Automaton& elr, std::ostream& out, std::string_view indent) {
		for(const auto& state : elr.states()) {
			print(state, out, indent);
		}
	}

	void print(const elr::State& state, std::ostream& out, std::string_view indent) {
		out << indent;
		if(state.isStartState()) {
			out << "=> ";
		}
		out << "( " << state.id() << " )";
		if(state.isReduceState()) {
			out << " => " << state.reduction().reduceRule;
		}
		out << '\n';

		for(const auto& trans : state.gotos()) {
			out << indent << "  " << trans << '\n';
		}

		for(const auto& trans : state.shifts()) {
			out << indent << "  " << trans << '\n';
		}

		for(const auto& backLink : state.backLinks()) {
			out << indent << "  ";
			if(state.isReduceState() && backLink == state.reduction().backLink) {
				out << "=> ";
			}
			out << backLink << '\n';
		}
	}
	


	void print(const dfa::Automaton& dfa, std::ostream& out, std::string_view indent) {
		for(const auto& state : dfa.states()) {
			print(state, out, indent);
		}
	}

	void print(const dfa::State& state, std::ostream& out, std::string_view indent) {
		out << indent;
		if(state.isStartState()) {
			out << "=> ";
		}
		out << "( " << state.id() << " )";
		if(state.isMatchState()) {
			out << " => " << state.matchedRule();
		}
		out << '\n';

		for(const auto& trans : state.transitions()) {
			out << indent << "  " << trans << '\n';
		}
	}

	void print(const SymbolGrammar& grammar, std::ostream& out, std::string_view indent) {
		for(const auto& rule : grammar.rules()) {
			out << indent << rule << '\n';
		}
	}



	std::ostream& operator<<(std::ostream& out, const elr::ReduceAction& reduce) {
		return out << '(' << reduce.reduceRule << ", " << reduce.backLink << ')';
	}

	std::ostream& operator<<(std::ostream& out, const elr::StateTrans& trans) {
		return out << trans.label << " => ( " << trans.target << " )";
	}



	std::ostream& operator<<(std::ostream& out, const dfa::StateTrans& trans) {
		return out << trans.label << " => ( " << trans.target << " )";
	}



	std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
		out << loc.lineNo() + 1 << ':' << loc.startCol() + 1;
		if(loc.colCount()) {
			out << '-' << (loc.endCol() - 1) + 1;
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const RegularExpr& expr) {
		if(expr) {
			out << *static_cast<const regex::NodePtr&>(expr);
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const SymbolRule& rule) {
		return out << rule.head() << ": " << rule.body();
	}

	std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
		return out << '\'' << symbol.value() << '\'';
	}
	


	std::ostream& operator<<(std::ostream& out, pars::TokenKinds tok) {
		switch(tok) {

#define PARSEC_PARS_TOKEN_KIND(tok, comment) case pars::TokenKinds::tok: out << #tok; break;

			PARSEC_PARS_TOKEN_KIND_LIST

#undef PARSEC_PARS_TOKEN_KIND
			
			default: out << "UnknownTokenType"; break;
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const pars::Token& tok) {
		return out << '(' << tok.kind() << ": " << '"' << tok.text() << "\")";
	}



	std::ostream& operator<<(std::ostream& out, const regex::ExprNode& n) {
		using namespace regex;

		class Impl : private NodeVisitor {
		public:

			Impl(std::ostream& out)
				: m_out(out) {}

			void operator()(const ExprNode& n) {
				n.accept(*this);
			}

		private:
			void visit(const SymbolAtom& n) override {
				if(n.value()) {
					m_out << n.value();
				} else {
					m_out << "()";
				}
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


			std::ostream& m_out;
		} impl(out);
		impl(n);
		
		return out;
	}
}