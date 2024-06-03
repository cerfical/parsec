#pragma once

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
