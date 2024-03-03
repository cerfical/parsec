#ifndef PARSEC_FG_RULE_EXPR_HEADER
#define PARSEC_FG_RULE_EXPR_HEADER

#include "../core/NonCopyable.hpp"

#include <memory>
#include <vector>
#include <string>
#include <ostream>

namespace parsec::fg {

	/**
	 * @brief Defines a relationship between symbolic names using trivial formal language operations.
	*/
	class RuleExpr : private NonCopyable {
	public:

		friend std::ostream& operator<<(std::ostream& out, const RuleExpr& expr) {
			if(expr.m_rootNode) {
				expr.m_rootNode->printTo(out);
			}
			return out;
		}



		/**
		 * @brief Create an empty rule.
		*/
		RuleExpr() = default;
		


		/**
		 * @brief Create a simple rule with a single symbol.
		*/
		explicit RuleExpr(std::string symbol);



		/** @{ */
		/**
		 * @brief Combine one rule with another, creating a rule concatenation.
		*/
		void concat(RuleExpr other);
		


		/**
		 * @brief Combine one rule with another, creating a rule alternation.
		*/
		void altern(RuleExpr other);



		/**
		 * @brief Create a Kleene plus repetition rule.
		*/
		void repeat();
		/** @} */



		/** @{ */
		/**
		 * @brief List of symbols that follow another symbol.
		*/
		std::vector<std::size_t> followPos(std::size_t i) const;
		
		
		
		/**
		 * @brief List of symbols that start the rule.
		*/
		std::vector<std::size_t> firstPos() const;



		/**
		 * @brief Get a symbol located at the specified position index.
		*/
		const std::string& symbolAt(std::size_t i) const;
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


			virtual void computeFirstPos(std::vector<std::size_t>& pos) const = 0;

			virtual SearchResult computeFollowPos(const Symbol* symbol, std::vector<std::size_t>& pos) const = 0;
			
			virtual bool isNullable() const noexcept = 0;


			virtual void printTo(std::ostream& out) const = 0;

		};

		using NodePtr = std::unique_ptr<Node>;



		template <typename Node, typename... Args>
		static auto makeNode(Args&&... args) {
			return std::make_unique<Node>(std::forward<Args>(args)...);
		}

		void mergeSymbols(std::vector<Symbol*>&);



		std::vector<Symbol*> m_symbols;
		NodePtr m_rootNode;
	};

}

#endif