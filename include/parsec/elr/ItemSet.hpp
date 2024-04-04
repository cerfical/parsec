#ifndef PARSEC_ELR_ITEM_SET_HEADER
#define PARSEC_ELR_ITEM_SET_HEADER

#include "../core/NonCopyable.hpp"
#include "../hash.hpp"

#include "StateItem.hpp"

#include <vector>
#include <unordered_set>
#include <span>

namespace parsec::elr {

	class TransNetwork;

	class ItemSet : private NonCopyable {
		friend std::size_t parsec::hash(const ItemSet&) noexcept;

	public:

		friend bool operator==(const ItemSet& lhs, const ItemSet& rhs) noexcept {
			return lhs.m_items == rhs.m_items;
		}


		void insert(const StateItem& item);


		std::span<const StateItem> items() const;

		bool contains(const StateItem& item) const {
			return m_items.contains(item);
		}

		ItemSet closure(const TransNetwork& transNet) const;


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_items.empty();
		}


	private:
		struct ItemHasher {
			std::size_t operator()(const StateItem& item) const noexcept {
				return hash(item);
			}
		};
		
		std::unordered_set<StateItem, ItemHasher> m_items;
		
		mutable std::vector<StateItem> m_sortedItems;
		mutable bool m_itemsSorted = true;
	};

}

#endif