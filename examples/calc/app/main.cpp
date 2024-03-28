#include <parsec/parsec.hpp>

#include <sstream>
#include <iostream>
#include <stack>

#include "ExprParser.hpp"

class ExprEvaluator : Parser {
public:
	double eval(std::string_view expr) {
		parse(expr);
		
		const auto result = m_evalStack.top();
		m_evalStack.pop();
		return result;
	}

private:
	void onFactor(std::span<const Token> tokens) override {
		if(tokens.front().kind() == TokenKinds::Number) {
			m_evalStack.push(std::stod(tokens.front().text()));
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
		const double rhs = m_evalStack.top();
		m_evalStack.pop();

		const double lhs = m_evalStack.top();
		m_evalStack.pop();

		double result = 0;
		switch(op) {
			case TokenKinds::AddOp: result = lhs + rhs; break;
			case TokenKinds::SubOp: result = lhs - rhs; break;
			case TokenKinds::MulOp: result = lhs * rhs; break;
			case TokenKinds::DivOp: result = lhs / rhs; break;
		}
		m_evalStack.push(result);
	}


	std::stack<double> m_evalStack;
};

int main(int argc, const char* argv[]) {
	std::string expr;
	for(int i = 1; i < argc; i++) {
		expr += argv[i];
	}

	if(!expr.empty()) {
		try {
			const auto result = ExprEvaluator().eval(expr);
			std::cout << expr << " = " << result << '\n';
		} catch(...) {
			std::cout << "error: invalid input" << '\n';
		}
	} else {
		std::cerr << "error: empty input" << '\n';
	}
	return 1;
}
