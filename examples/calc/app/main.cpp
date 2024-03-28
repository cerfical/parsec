#include <parsec/parsec.hpp>
#include <parsec/deps.hpp>

#include <gsl/gsl>

#include <sstream>
#include <stack>

#include "ExprParser.hpp"

class ExprEvaluator : Parser {
public:
	double eval(const std::string& expr) {
		std::istringstream in(expr);
		parse(in);
		
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

int main(int argc, gsl::czstring argv[]) {
	std::string expr;
	for(const auto i : std::views::iota(1, argc)) {
		expr += argv[i];
	}

	try {
		std::cout << ExprEvaluator().eval(expr) << '\n';
	} catch(const std::exception& e) {
		std::cout << "fatal error: " << e.what() << '\n';
	}
}
