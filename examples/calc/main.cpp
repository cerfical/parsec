#include <parsec/parsec.hpp>

#include "ExprParser.hpp"

#include <iostream>
#include <spanstream>
#include <stack>

class ExprEvaluator : Parser {
public:

    explicit ExprEvaluator(std::string_view expr)
        : Parser(&expr_), expr_(expr) {}

    double eval() {
        parse();

        const auto result = evalStack_.top();
        evalStack_.pop();
        return result;
    }

private:
    void onFactor(std::span<const Token> tokens) override {
        if(tokens.front().kind() == TokenKinds::Number) {
            evalStack_.push(std::stod(tokens.front().text()));
        }
    }

    void onExpr(std::span<const Token> tokens) override {
        if(tokens.size() == 1) {
            evalBinary(tokens.front().kind());
        }
    }

    void onTerm(std::span<const Token> tokens) override {
        if(tokens.size() == 1) {
            evalBinary(tokens.front().kind());
        }
    }


    void evalBinary(TokenKinds op) {
        const double rhs = evalStack_.top();
        evalStack_.pop();

        const double lhs = evalStack_.top();
        evalStack_.pop();

        double result = 0;
        switch(op) {
            case TokenKinds::AddOp: result = lhs + rhs; break;
            case TokenKinds::SubOp: result = lhs - rhs; break;
            case TokenKinds::MulOp: result = lhs * rhs; break;
            case TokenKinds::DivOp: result = lhs / rhs; break;
            default:                break;
        }
        evalStack_.push(result);
    }


    std::stack<double> evalStack_;
    std::ispanstream expr_;
};

int main(int argc, const char* argv[]) {
    std::string expr;
    for(int i = 1; i < argc; i++) {
        expr += argv[i];
    }

    if(!expr.empty()) {
        try {
            const auto result = ExprEvaluator(expr).eval();
            std::cout << expr << " = " << result << '\n';
        } catch(const std::exception& e) {
            std::cout << "error: invalid input: " << e.what() << '\n';
        } catch(...) {
            std::cout << "error: encountered an unknown problem" << '\n';
        }
    } else {
        std::cerr << "error: empty input" << '\n';
    }
    return 1;
}
