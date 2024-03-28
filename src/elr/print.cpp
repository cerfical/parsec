#include "elr.hpp"
#include "dfa/print.hpp"

namespace parsec::elr {
	void print(const State& state, std::ostream& out, std::string_view indent) {
		out << indent;
		if(state.isStartState()) {
			out << "=> ";
		}
		out << state;
		if(state.isReduceState()) {
			out << " => " << state.reduction().reduceRule();
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
			if(state.isReduceState() && backLink == state.reduction().backLink()) {
				out << "=> ";
			}
			out << backLink << '\n';
		}
	}


	void print(const Automaton& elr, std::ostream& out, std::string_view indent) {
		for(const auto& state : elr.states()) {
			print(state, out, indent);
		}
	}


	void print(const TransNetwork& transNet, std::ostream& out, std::string_view indent) {
		for(const auto& state : transNet.states()) {
			print(state, out, indent);
		}
	}


	void print(const ItemSet& itemSet, std::ostream& out, std::string_view indent) {
		for(const auto& item : itemSet.items()) {
			out << indent << item << '\n';
		}
	}


	std::ostream& operator<<(std::ostream& out, const ReduceAction& reduce) {
		return out << "( " << reduce.reduceRule() << ", " << reduce.backLink() << " )";
	}


	std::ostream& operator<<(std::ostream& out, const StateTrans& trans) {
		return out << trans.label() << " => " << State(trans.target());
	}


	std::ostream& operator<<(std::ostream& out, const State& state) {
		return out << "( " << state.id() << " )";
	}


	std::ostream& operator<<(std::ostream& out, const StateItem& item) {
		return out << "( " << item.dfaState() << ", " << item.backLink() << " )";
	}
}