#ifndef PARSEC_OPTION_STORE_HEADER
#define PARSEC_OPTION_STORE_HEADER

#include "OptionBuilder.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gsl/gsl>

#include <string_view>
#include <iostream>

namespace parsec::cli {
	/**
	 * @brief Analyzes and caches various program configuration options.
	 */
	class OptionStore {
		friend OptionBuilder;

	public:
		/** @{ */
		/** @brief Create and initialize a new store to read options from the command line. */
		OptionStore(int argc, gsl::czstring argv[])
			: m_named("Options"), m_argv(argv), m_argc(argc)
		{ }


		/** @brief Clear and destroy the store. */
		~OptionStore() = default;
		/** @} */


		/** @{ */
		OptionStore(OptionStore&&) = delete;
		OptionStore& operator=(OptionStore&&) = delete;
		/** @} */


		/** @{ */
		OptionStore(const OptionStore&) = delete;
		OptionStore& operator=(const OptionStore&) = delete;
		/** @} */


		/** @{ */
		/** @brief Register a new named option to search for on the command line. */
		OptionBuilder reg(std::string_view name) {
			return OptionBuilder(*this, name);
		}


		/** @brief Looks up the specified option on the command line and returns its value. */
		template <typename T>
		const T& get(std::string_view name, const T& value = T()) const {
			updateParsed();

			const auto it = m_parsed.find(std::string(name));
			if(it != m_parsed.cend()) {
				if(!it->second.empty()) {
					return it->second.as<T>();
				}
			}

			return value;
		}


		/** @brief Check if the option is present. */
		bool check(std::string_view name) const {
			return m_parsed.contains(std::string(name));
		}
		/** @} */


		/** @{ */
		/** @brief Prints out the information about available options. */
		void dump(std::ostream& out = std::cout) const {
			out << m_named;
		}
		/** @} */


	private:
		/** @{ */
		void updateParsed() const;
		/** @} */

		boost::program_options::positional_options_description m_positional;
		boost::program_options::options_description m_named;

		mutable boost::program_options::variables_map m_parsed;
		mutable bool m_changed = false;

		gsl::czstring* m_argv = nullptr;
		int m_argc = 0;
	};
}

#endif