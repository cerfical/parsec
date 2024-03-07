#ifndef PARSEC_FG_RULE_HEADER
#define PARSEC_FG_RULE_HEADER

#include "../core/NonCopyable.hpp"
#include "../core/typedefs.hpp"

#include <memory>
#include <vector>
#include <string_view>
#include <ostream>
#include <string>

namespace parsec::fg {

	/**
	 * @brief Defines a relationship between symbols using trivial formal language operations.
	*/
	class Rule : private NonCopyable {
	public:

		/**
		 * @brief Create an empty rule.
		*/
		Rule() = default;
		


		/**
		 * @brief Create a simple rule with a single symbol.
		*/
		explicit Rule(std::string_view symbol);



		/** @{ */
		/**
		 * @brief Combine one rule with another, creating a rule concatenation.
		*/
		void concat(Rule other);
		


		/**
		 * @brief Combine one rule with another, creating a rule alternation.
		*/
		void altern(Rule other);



		/**
		 * @brief Make the rule a repetition with a Kleene plus operator.
		*/
		void repeatPlus();



		/**
		 * @brief Make the rule a repetition with a Kleene star operator.
		*/
		void repeatStar() {
			repeatPlus();
			altern({});
		}



		/**
		 * @brief Make the rule optional.
		*/
		void markOptional() {
			altern({});
		}
		/** @} */



		/** @{ */
		/**
		 * @brief List of symbols that follow another symbol.
		*/
		IndexList followPos(Index pos) const;
		
		
		
		/**
		 * @brief List of symbols that start the rule.
		*/
		IndexList firstPos() const;



		/**
		 * @brief Get a symbol located at the specified position index.
		*/
		std::string_view symbolAt(Index pos) const;



		/**
		 * @brief Convert the rule to its equivalent string form.
		*/
		std::string toStr() const;
		/** @} */



	private:
		class Symbol;
		class EndSymbol;
		class Nil;
		class Repeat;
		class Binary;
		class Concat;
		class Altern;

		class Node {
		public:
			enum class SearchResult {
				Finished,
				Found,
				NotFound
			};

			virtual ~Node() = default;

			virtual void computeFirstPos(IndexList& posList) const = 0;
			virtual SearchResult computeFollowPos(const Symbol* pos, IndexList& posList) const = 0;
			virtual bool isNullable() const noexcept = 0;

			virtual void printTo(std::ostream& out) const = 0;
		};

		using NodePtr = std::unique_ptr<Node>;


		void mergeSymbols(std::vector<Symbol*>&);

		std::vector<Symbol*> m_symbols;
		NodePtr m_rootNode;
	};



	inline std::ostream& operator<<(std::ostream& out, const Rule& expr) {
		out << expr.toStr();
		return out;
	}

}

#endif