#ifndef PARSEC_CPP_SRC_GEN_HEADER
#define PARSEC_CPP_SRC_GEN_HEADER

#include "Grammar.hpp"
#include <iostream>

namespace parsec {
	/**
	 * @brief Generates C++ source code for a @ref Grammar "grammar"-defined parser description.
	 */
	class CppSrcGen {
	public:
		explicit CppSrcGen(std::ostream& out = std::cout) noexcept
			: m_out(&out)
		{ }


		/** @{ */
		CppSrcGen(CppSrcGen&&) = default;
		CppSrcGen& operator=(CppSrcGen&&) = default;
		/** @} */

		/** @{ */
		CppSrcGen(const CppSrcGen&) = delete;
		CppSrcGen& operator=(const CppSrcGen&) = delete;
		/** @} */
		
		
		/** @{ */
		void run(const Grammar& grammar);
		/** @} */


	private:
		std::ostream* m_out;
	};
}

#endif