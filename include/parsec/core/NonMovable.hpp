#ifndef PARSEC_CORE_NON_MOVABLE_HEADER
#define PARSEC_CORE_NON_MOVABLE_HEADER

#include "NonCopyable.hpp"

namespace parsec {

	/**
	 * @brief Mixin type to put further restrictions on NonCopyable types by disabling move semantics.
	*/
	struct NonMovable : private NonCopyable {

		NonMovable() = default;
		~NonMovable() = default;

		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;

	};

}

#endif