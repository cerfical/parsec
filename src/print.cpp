#include "print.hpp"

#include "core/RegularExpr.hpp"
#include "core/SourceLoc.hpp"
#include "core/Symbol.hpp"
#include "core/SymbolGrammar.hpp"
#include "core/SymbolRule.hpp"

#include "pars/Token.hpp"
#include "pars/TokenKinds.hpp"

#include "regex/nodes/AlternExpr.hpp"
#include "regex/nodes/ConcatExpr.hpp"
#include "regex/nodes/NodeVisitor.hpp"
#include "regex/nodes/OptionalExpr.hpp"
#include "regex/nodes/PlusClosure.hpp"
#include "regex/nodes/StarClosure.hpp"
#include "regex/nodes/SymbolAtom.hpp"

namespace parsec::regex {
    std::ostream& operator<<(std::ostream& out, const ExprNode& n) {
        class Impl : private NodeVisitor {
        public:

            Impl(std::ostream& out)
                : out_(out) {}

            void operator()(const ExprNode& n) {
                n.accept(*this);
            }

        private:
            void visit(const SymbolAtom& n) override {
                if(n.value()) {
                    out_ << n.value();
                } else {
                    out_ << "()";
                }
            }

            void visit(const OptionalExpr& n) override {
                n.inner()->accept(*this);
                out_ << '?';
            }

            void visit(const PlusClosure& n) override {
                n.inner()->accept(*this);
                out_ << '+';
            }

            void visit(const StarClosure& n) override {
                n.inner()->accept(*this);
                out_ << '*';
            }

            void visit(const AlternExpr& n) override {
                out_ << '(';
                n.left()->accept(*this);
                out_ << " | ";
                n.right()->accept(*this);
                out_ << ')';
            }

            void visit(const ConcatExpr& n) override {
                out_ << '(';
                n.left()->accept(*this);
                out_ << ' ';
                n.right()->accept(*this);
                out_ << ')';
            }


            std::ostream& out_;
        } impl(out);
        impl(n);

        return out;
    }
}

namespace parsec::pars {
    std::ostream& operator<<(std::ostream& out, TokenKinds tok) {
        switch(tok) {
            case TokenKinds::EmptyToken: out << "EmptyToken"; break;
            case TokenKinds::Eof:        out << "Eof"; break;

            case TokenKinds::Ident:         out << "Ident"; break;
            case TokenKinds::PatternString: out << "PatternString"; break;

            case TokenKinds::Star:         out << "Star"; break;
            case TokenKinds::Plus:         out << "Plus"; break;
            case TokenKinds::QuestionMark: out << "QuestionMark"; break;
            case TokenKinds::Pipe:         out << "Pipe"; break;
            case TokenKinds::Semicolon:    out << "Semicolon"; break;
            case TokenKinds::Equals:       out << "Equals"; break;

            case TokenKinds::LeftBrace:  out << "LeftBrace"; break;
            case TokenKinds::RightBrace: out << "RightBrace"; break;

            case TokenKinds::LeftParen:  out << "LeftParen"; break;
            case TokenKinds::RightParen: out << "RightParen"; break;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Token& tok) {
        return out << '(' << tok.kind() << ": " << '"' << tok.text() << "\")";
    }
}

namespace parsec::fsm {
    std::ostream& operator<<(std::ostream& out, const StateTrans& trans) {
        return out << trans.label << " => ( " << trans.target << " )";
    }


    void print(const ElrAutomaton& elr, std::ostream& out, std::string_view indent) {
        for(const auto& state : elr.states()) {
            print(state, out, indent);
        }
    }

    void print(const ElrState& state, std::ostream& out, std::string_view indent) {
        out << indent;
        if(state.isStartState()) {
            out << "=> ";
        }
        out << "( " << state.id() << " )";
        if(state.isMatchState()) {
            out << " => " << state.match();
        }
        out << '\n';

        for(const auto& trans : state.gotos()) {
            out << indent << "  " << trans << '\n';
        }

        for(const auto& trans : state.shifts()) {
            out << indent << "  " << trans << '\n';
        }

        for(const auto& backlink : state.backlinks()) {
            out << indent << "  ";
            if(state.isMatchState() && backlink == state.backlink()) {
                out << "=> ";
            }
            out << backlink << '\n';
        }
    }


    void print(const DfaAutomaton& dfa, std::ostream& out, std::string_view indent) {
        for(const auto& state : dfa.states()) {
            print(state, out, indent);
        }
    }

    void print(const DfaState& state, std::ostream& out, std::string_view indent) {
        out << indent;
        if(state.isStartState()) {
            out << "=> ";
        }
        out << "( " << state.id() << " )";
        if(state.isMatchState()) {
            out << " => " << state.match();
        }
        out << '\n';

        for(const auto& trans : state.transitions()) {
            out << indent << "  " << trans << '\n';
        }
    }
}

namespace parsec {
    std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
        out << loc.line.no + 1 << ':' << loc.startCol() + 1;
        if(loc) {
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


    void print(const SymbolGrammar& grammar, std::ostream& out, std::string_view indent) {
        for(const auto& rule : grammar.rules()) {
            out << indent << rule << '\n';
        }
    }
}
