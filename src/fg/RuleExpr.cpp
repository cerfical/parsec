#include "fg/RuleExpr.hpp"

#include "RuleExpr_nodes.hpp"
#include <sstream>

namespace parsec::fg {
	RuleExpr::RuleExpr(std::string symbol) {
		if(!symbol.empty()) {
			auto rootNode = makeNode<Symbol>();
			m_symbols.push_back(rootNode.get());

			rootNode->name = std::move(symbol);
			m_rootNode = std::move(rootNode);
		}
	}


	void RuleExpr::mergeSymbols(std::vector<Symbol*>& symbols) {
		const auto symbolPosOffset = m_symbols.size();

		auto it = m_symbols.insert(m_symbols.end(), symbols.begin(), symbols.end());
		for(; it != m_symbols.end(); it++) {
			(*it)->posIndex += symbolPosOffset;
		}
	}


	void RuleExpr::concat(RuleExpr other) {
		if(m_rootNode && other.m_rootNode) {
			auto newRootNode = makeNode<Concat>();
			mergeSymbols(other.m_symbols);

			newRootNode->left = std::move(m_rootNode);
			newRootNode->right = std::move(other.m_rootNode);
			m_rootNode = std::move(newRootNode);
		} else if(other.m_rootNode) {
			*this = std::move(other);
		}
	}


	void RuleExpr::altern(RuleExpr other) {
		if(m_rootNode || other.m_rootNode) {
			// ugly code needed to provide strong exception guarantees
			NodePtr *left, leftNil, *right, rightNil;
			std::tie(left, leftNil) = m_rootNode ?
				std::tuple(&m_rootNode, nullptr) : std::tuple(&leftNil, makeNode<Nil>());
			
			std::tie(right, rightNil) = other.m_rootNode ?
				std::tuple(&other.m_rootNode, nullptr) : std::tuple(&rightNil, makeNode<Nil>());


			auto newRootNode = makeNode<Altern>();
			mergeSymbols(other.m_symbols);

			newRootNode->left = std::move(*left);
			newRootNode->right = std::move(*right);
			m_rootNode = std::move(newRootNode);
		}
	}


	void RuleExpr::repeat() {
		if(m_rootNode) {
			auto newRootNode = makeNode<Repeat>();
			newRootNode->inner = std::move(m_rootNode);
			m_rootNode = std::move(newRootNode);
		}
	}


	IndexList RuleExpr::followPos(Index pos) const {
		if(m_rootNode && pos < m_symbols.size()) {
			return EndSymbol(m_rootNode.get(), m_symbols.size())
				.followPosOf(m_symbols[pos]);
		}
		return {};
	}


	IndexList RuleExpr::firstPos() const {
		if(m_rootNode) {
			return EndSymbol(m_rootNode.get(), m_symbols.size())
				.firstPos();
		}
		return { 0 };
	}


	const std::string& RuleExpr::symbolAt(Index pos) const {
		if(pos < m_symbols.size()) {
			return m_symbols[pos]->name;
		}

		static std::string empty;
		return empty;
	}


	std::string RuleExpr::toStr() const {
		std::ostringstream out;
		if(m_rootNode) {
			m_rootNode->printTo(out);
		}
		return out.str();
	}
}