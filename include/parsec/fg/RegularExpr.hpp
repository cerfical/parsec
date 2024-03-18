#ifndef PARSEC_FG_REGULAR_EXPR_HEADER
#define PARSEC_FG_REGULAR_EXPR_HEADER

#include "regex/ExprNode.hpp"
#include "regex/SymbolAtom.hpp"
#include "Symbol.hpp"

#include <vector>

namespace parsec::fg {

	class RegularExpr {
	public:

		class Atom;

		using AtomList = std::vector<Atom>;



		friend RegularExpr altern(const RegularExpr& left, const RegularExpr& right);

		friend RegularExpr concat(const RegularExpr& left, const RegularExpr& right);
		
		friend RegularExpr starClosure(const RegularExpr& expr);
		
		friend RegularExpr plusClosure(const RegularExpr& expr);
		
		friend RegularExpr optional(const RegularExpr& expr);



		static RegularExpr fromPatternString(std::string_view str);

		static RegularExpr fromRawString(std::string_view str);



		RegularExpr(const Symbol& symbol)
			: RegularExpr(regex::makeNode<regex::SymbolAtom>(symbol)) {}

		RegularExpr() = default;
		


		std::string toString() const;

		AtomList firstAtoms() const;


		explicit operator bool() const {
			return !isEmpty();
		}

		bool isEmpty() const {
			return m_rootNode == nullptr;
		}



		RegularExpr& operator|=(const RegularExpr& other) {
			return *this = altern(*this, other);
		}

		RegularExpr& operator+=(const RegularExpr& other) {
			return *this = concat(*this, other);
		}



	private:
		static void computeFirstPos(regex::NodePtr, const regex::ExprNode*, AtomList&, std::size_t&);

		explicit RegularExpr(regex::NodePtr rootNode)
			: m_rootNode(rootNode) {}

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
		return concat(left, right);
	}

	inline RegularExpr operator|(const RegularExpr& left, const RegularExpr& right) {
		return altern(left, right);
	}

	inline std::ostream& operator<<(std::ostream& out, const RegularExpr& expr) {
		return out << expr.toString();
	}



	RegularExpr altern(const RegularExpr& left, const RegularExpr& right);
	
	RegularExpr concat(const RegularExpr& left, const RegularExpr& right);
	
	RegularExpr starClosure(const RegularExpr& expr);
	
	RegularExpr plusClosure(const RegularExpr& expr);
	
	RegularExpr optional(const RegularExpr& expr);

}

#endif