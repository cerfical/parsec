#ifndef PARSEC_OPTION_BUILDER_HEADER
#define PARSEC_OPTION_BUILDER_HEADER

#include <boost/program_options/value_semantic.hpp>

#include <string_view>
#include <memory>

namespace parsec::cli {
	class OptionStore;

	/**
	 * @brief Used to configure and register a new command line option in a OptionStore.
	 */
	class OptionBuilder {
	public:
		/** @{ */
		/** @brief Create a builder to configure a new named option. */
		OptionBuilder(OptionStore& options, std::string_view name);


		/** @brief Completes the option creation and registers it in its owning OptionStore. */
		~OptionBuilder();
		/** @} */


		/** @{ */
		OptionBuilder(const OptionBuilder&) = delete;
		OptionBuilder& operator=(const OptionBuilder&) = delete;
		/** @} */


		/** @{ */
		OptionBuilder(OptionBuilder&&) = delete;
		OptionBuilder& operator=(OptionBuilder&&) = delete;
		/** @} */


		/** @{ */
		/** @brief Specify the default value that will be assigned to the option if a value is not explicitly stated. */
		template <typename T>
		OptionBuilder& valued(const T& val) {
			m_value.reset(
				boost::program_options::value<T>()->default_value(val)
			);
			return *this;
		}


		/** @brief Allow the option to have an optional typed value. */
		template <typename T>
		OptionBuilder& typed() {
			m_value.reset(boost::program_options::value<T>());
			return *this;
		}


		/** @brief Allow option to be specified by a value in a proper position. */
		OptionBuilder& positional() {
			m_positional = true;
			return *this;
		}


		/** @brief Brief description of the option's effects. */
		OptionBuilder& desc(std::string_view desc) {
			m_desc = desc;
			return *this;
		}
		/** @} */


	private:
		std::unique_ptr<boost::program_options::value_semantic> m_value;
		std::string m_name, m_desc;
		bool m_positional = false;

		OptionStore& m_options;
	};
}

#endif