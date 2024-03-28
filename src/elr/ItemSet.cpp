#include "elr/ItemSet.hpp"

#include "elr/TransNetwork.hpp"
#include <algorithm>

namespace parsec::elr {
	namespace {
		void closeItem(const StateItem& item, const TransNetwork& transNet, ItemSet& closure) {
			if(closure.contains(item)) {
				return;
			}
			closure.insert(item);

			for(const auto& trans : transNet.stateById(item.dfaState()).transitions()) {
				if(const auto& startState = transNet.startState(trans.label())) {
					closeItem(startState.id(), transNet, closure);
				}
			}
		}
	}


	void ItemSet::insert(const StateItem& item) {
		const auto [itemIt, wasInserted] = m_items.insert(item);
		if(wasInserted) {
			try {
				m_sortedItems.push_back(item);
			} catch(...) {
				m_items.erase(itemIt);
				throw;
			}
			m_itemsSorted = false;
		}
	}


	std::span<const StateItem> ItemSet::items() const {
		if(!m_itemsSorted) {
			std::ranges::sort(m_sortedItems);
			m_itemsSorted = true;
		}
		return m_sortedItems;
	}


	ItemSet ItemSet::closure(const TransNetwork& transNet) const {
		ItemSet closedSet;
		for(const auto& item : items()) {
			closeItem(item, transNet, closedSet);
		}
		return closedSet;
	}
}