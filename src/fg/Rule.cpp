#include "fg/Rule.hpp"

#include "Rule-nodes.hpp"
#include <sstream>

namespace parsec::fg {
	Rule::Rule(std::string_view symbol) {
		if(!symbol.empty()) {
			auto rootNode = std::make_unique<Symbol>();
			rootNode->name = symbol;
			m_symbols.push_back(rootNode.get());

			m_rootNode = std::move(rootNode);
		}
	}


	void Rule::concat(Rule other) {
		if(m_rootNode && other.m_rootNode) {
			auto newRootNode = std::make_unique<Concat>();
			mergeSymbols(other.m_symbols);

			newRootNode->left = std::move(m_rootNode);
			newRootNode->right = std::move(other.m_rootNode);
			m_rootNode = std::move(newRootNode);
		} else if(other.m_rootNode) {
			*this = std::move(other);
		}
	}


	void Rule::altern(Rule other) {
		if(m_rootNode || other.m_rootNode) {
			// ugly code needed to provide strong exception guarantees
			NodePtr* left, leftNil, * right, rightNil;
			std::tie(left, leftNil) = m_rootNode ?
				std::tuple(&m_rootNode, nullptr) : std::tuple(&leftNil, std::make_unique<Nil>());

			std::tie(right, rightNil) = other.m_rootNode ?
				std::tuple(&other.m_rootNode, nullptr) : std::tuple(&rightNil, std::make_unique<Nil>());


			auto newRootNode = std::make_unique<Altern>();
			mergeSymbols(other.m_symbols);

			newRootNode->left = std::move(*left);
			newRootNode->right = std::move(*right);
			m_rootNode = std::move(newRootNode);
		}
	}


	void Rule::repeatPlus() {
		if(m_rootNode) {
			auto newRootNode = std::make_unique<Repeat>();
			newRootNode->inner = std::move(m_rootNode);
			m_rootNode = std::move(newRootNode);
		}
	}


	IndexList Rule::followPos(Index pos) const {
		if(m_rootNode && pos < m_symbols.size()) {
			return EndSymbol(m_rootNode.get(), m_symbols.size())
				.followPosOf(m_symbols[pos]);
		}
		return {};
	}


	IndexList Rule::firstPos() const {
		if(m_rootNode) {
			return EndSymbol(m_rootNode.get(), m_symbols.size())
				.firstPos();
		}
		return { 0 };
	}


	std::string_view Rule::symbolAt(Index pos) const {
		if(pos < m_symbols.size()) {
			return m_symbols[pos]->name;
		}
		return "";
	}


	std::string Rule::toStr() const {
		std::ostringstream out;
		if(m_rootNode) {
			m_rootNode->printTo(out);
		}
		return out.str();
	}


	void Rule::mergeSymbols(std::vector<Symbol*>& symbols) {
		const auto symbolPosOffset = m_symbols.size();

		auto it = m_symbols.insert(m_symbols.end(), symbols.begin(), symbols.end());
		for(; it != m_symbols.end(); it++) {
			(*it)->posIndex += symbolPosOffset;
		}
		symbols.clear();
	}
}