#ifndef PARSEC_PRINT_HEADER
#define PARSEC_PRINT_HEADER

#include <iostream>
#include <string_view>

namespace parsec {

	class RegularExpr;
	class SymbolRule;
	class Symbol;

	namespace pars {
		enum class TokenKinds;
		class Token;
	}

	namespace regex {
		class ExprNode;
	}

	namespace dfa {
		class State;
		class Automaton;
		class StateTrans;
	}

	namespace elr {
		class Automaton;
		class TransNetwork;
		class ItemSet;
		class ReduceAction;
		class State;
		class StateTrans;
		class StateItem;
	}



	/**
	 * @name Printing simple objects to streams
	 * @{
	*/
	std::ostream& operator<<(std::ostream& out, const dfa::StateTrans& trans);



	std::ostream& operator<<(std::ostream& out, const elr::ReduceAction& reduce);

	std::ostream& operator<<(std::ostream& out, const elr::StateTrans& trans);

	std::ostream& operator<<(std::ostream& out, const elr::StateItem& item);



	std::ostream& operator<<(std::ostream& out, const RegularExpr& expr);
	
	std::ostream& operator<<(std::ostream& out, const SymbolRule& rule);

	std::ostream& operator<<(std::ostream& out, const Symbol& symbol);



	std::ostream& operator<<(std::ostream& out, pars::TokenKinds tok);
	
	std::ostream& operator<<(std::ostream& out, const pars::Token& tok);



	std::ostream& operator<<(std::ostream& out, const regex::ExprNode& n);
	/** @} */



	/**
	 * @name Printing objects with more elaborate structure
	 * @{
	*/
	void print(const dfa::State& state, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const dfa::Automaton& dfa, std::ostream& out = std::cout, std::string_view indent = "");



	void print(const elr::State& state, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const elr::Automaton& elr, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const elr::TransNetwork& transNet, std::ostream& out = std::cout, std::string_view indent = "");

	void print(const elr::ItemSet& itemSet, std::ostream& out = std::cout, std::string_view indent = "");
	/** @} */

}

#endif