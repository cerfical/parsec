#pragma once

#include <cstddef>

namespace parsec {

	class Symbol;


	/**
	 * @name Finding hash values for objects
	 * @{
	*/
	std::size_t hash(const Symbol& symbol) noexcept;
	/** @} */

}
