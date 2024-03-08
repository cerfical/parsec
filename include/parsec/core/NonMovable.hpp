#ifndef PARSEC_CORE_NON_MOVABLE_HEADER
#define PARSEC_CORE_NON_MOVABLE_HEADER

namespace parsec {

	/**
	 * @brief Mixin type to put further restrictions on types by disabling move semantics.
	*/
	struct NonMovable {

		NonMovable() = default;
		~NonMovable() = default;

		NonMovable(const NonMovable&) = default;
		NonMovable& operator=(const NonMovable&) = default;

		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;

	};

}

#endif