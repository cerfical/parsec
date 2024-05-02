#ifndef PARSEC_SRC_GEN_CONFIG_STORE_HEADER
#define PARSEC_SRC_GEN_CONFIG_STORE_HEADER

#include "../utils/util_types.hpp"

#include <string>
#include <string_view>
#include <array>

namespace parsec::src_gen {

	/**
	 * @brief Contains a set of various settings to customize the output produced by a generator.
	*/
	class ConfigStore : public NonCopyable {
	public:

		/**
		 * @brief Name for a special token marking the end of file.
		*/
		ConfigStore& eofTokenName(std::string_view value) {
			m_data[0] = value;
			return *this;
		}

		std::string_view eofTokenName() const {
			return m_data[0];
		}



		/**
		 * @brief Name for a token that defines which character sequences are considered whitespace.
		*/
		ConfigStore& wsTokenName(std::string_view value) {
			m_data[1] = value;
			return *this;
		}
		
		std::string_view wsTokenName() const {
			return m_data[1];
		}



	private:
		std::array<std::string, 2> m_data = {
		   "Eof",
		   "Ws"
		};
	};

}

#endif