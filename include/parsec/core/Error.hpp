#pragma once

#include <stdexcept>

namespace parsec {

	class Error : public std::runtime_error {
	public:

		explicit Error(const std::string& msg)
			: runtime_error(msg) {}

	};

}
