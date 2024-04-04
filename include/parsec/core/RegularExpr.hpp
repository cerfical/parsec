#ifndef PARSEC_CORE_REGULAR_EXPR_HEADER
#define PARSEC_CORE_REGULAR_EXPR_HEADER

#include "../regex/nodes/ExprNode.hpp"
#include "../regex/make.hpp"

#include "Symbol.hpp"
#include <span>

namespace parsec {

	class RegularExpr {
	public:

		RegularExpr() = default;

		explicit RegularExpr(std::string_view regex);

		RegularExpr(regex::NodePtr regex)
			: m_regex(regex) {}
		


		std::span<const int> firstPos() const;

		std::span<const int> followPos(int posIndex) const;

		const Symbol& posValue(int posIndex) const;

		bool isEndPos(int posIndex) const {
			return posValue(posIndex).isEmpty();
		}



		operator regex::NodePtr() const {
			return m_regex;
		}

		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_regex == nullptr;
		}



		RegularExpr& operator|=(const RegularExpr& other) {
			return *this = regex::altern(*this, other);
		}

		RegularExpr& operator+=(const RegularExpr& other) {
			return *this = regex::concat(*this, other);
		}



	private:
		struct ComputeCache;
		ComputeCache* computeCache() const;

		mutable std::shared_ptr<ComputeCache> m_computeCache;
		regex::NodePtr m_regex;
	};



	inline RegularExpr operator+(const RegularExpr& left, const RegularExpr& right) {
		return regex::concat(left, right);
	}

	inline RegularExpr operator|(const RegularExpr& left, const RegularExpr& right) {
		return regex::altern(left, right);
	}

}

#endif