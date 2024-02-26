#ifndef PARSEC_FSM_SYMBOL_HEADER
#define PARSEC_FSM_SYMBOL_HEADER

#include <string>

namespace parsec::fsm {
	
	/**
	 * @brief Abstraction of symbolic names that an Automaton will operate on.
	*/
	class Symbol {
	public:

		friend bool operator==(const Symbol&, const Symbol&) = default;


		Symbol() = default;
		
		Symbol(std::string name)
			: m_name(std::move(name)) {
		}

		Symbol(const char* name)
			: m_name(name) {
		}


		/** @{ */
		/**
		 * @brief Unique string identifier of the symbol.
		*/
		const std::string& name() const {
			return m_name;
		}


		/**
		 * @brief Check if the symbol has a meaningless empty name.
		*/
		bool isNil() const {
			return m_name.empty();
		}


		/**
		 * @brief Check if the symbol is not nil.
		*/
		operator bool() const {
			return !isNil();
		}
		/** @} */


	private:
		std::string m_name;
	};

}

#endif