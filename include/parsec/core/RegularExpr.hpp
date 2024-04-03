#ifndef PARSEC_CORE_REGULAR_EXPR_HEADER
#define PARSEC_CORE_REGULAR_EXPR_HEADER

#include "../regex/nodes/ExprNode.hpp"
#include "../regex/nodes/SymbolAtom.hpp"
#include "../regex/make.hpp"

#include <vector>

namespace parsec {

	class RegularExpr {
	public:

		class Atom;

		using AtomList = std::vector<Atom>;



		explicit RegularExpr(std::string_view regex);

		RegularExpr(regex::NodePtr rootNode)
			: m_rootNode(rootNode) {}
		
		RegularExpr() = default;
		


		AtomList firstAtoms() const;


		operator regex::NodePtr() const {
			return m_rootNode;
		}

		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_rootNode == nullptr;
		}



		RegularExpr& operator|=(const RegularExpr& other) {
			return *this = regex::altern(*this, other);
		}

		RegularExpr& operator+=(const RegularExpr& other) {
			return *this = regex::concat(*this, other);
		}



	private:
		static void computeFirstPos(regex::NodePtr, const regex::ExprNode*, AtomList&, std::size_t&);

		regex::NodePtr m_rootNode;
	};



	class RegularExpr::Atom {
		friend RegularExpr;

	public:

		AtomList followAtoms() const;

		const Symbol& symbol() const;

		std::size_t posIndex() const {
			return m_posIndex;
		}

		bool isEnd() const {
			return symbol().isEmpty(); 
		}

	private:
		Atom(std::size_t posIndex) noexcept
			: m_posIndex(posIndex) {}
		
		Atom(regex::NodePtr rootNode, const regex::SymbolAtom* atomNode, std::size_t posIndex) noexcept
			: m_rootNode(rootNode), m_atomNode(atomNode), m_posIndex(posIndex) {}

		regex::NodePtr m_rootNode;
		const regex::SymbolAtom* m_atomNode = {};
		std::size_t m_posIndex = {};
	};



	inline RegularExpr operator+(const RegularExpr& left, const RegularExpr& right) {
		return regex::concat(left, right);
	}

	inline RegularExpr operator|(const RegularExpr& left, const RegularExpr& right) {
		return regex::altern(left, right);
	}

}

#endif