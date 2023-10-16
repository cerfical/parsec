#ifndef PARSEC_CONSOLE_APP_HEADER
#define PARSEC_CONSOLE_APP_HEADER

#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gsl/gsl>

#include <string_view>
#include <iostream>
#include <memory>

namespace parsec::cli {
	/**
	 * @brief Facilitates writing console applications, making it easier, for example, to handle command line arguments.
	 */
	class ConsoleApp {
		class AppInstance;

	public:
		/** @{ */
		/**
		 * @brief Used to configure and register a new command line option.
		 */
		class OptionBuilder {
			friend ConsoleApp;

		public:
			/** @{ */
			/** @brief Completes the option creation and registers it in its owning object. */
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


			/** @brief Allow the option to be specified by a value in a proper position. */
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
			/** @{ */
			OptionBuilder(AppInstance& app, std::string_view name);
			/** @} */


			/** @{ */
			std::unique_ptr<boost::program_options::value_semantic> m_value;
			std::string m_name, m_desc;
			bool m_positional = false;
			/** @} */


			/** @{ */
			AppInstance& m_app;
			/** @} */
		};
		/** @} */


		/** @{ */
		/** @brief Initialize a new application instance. */
		ConsoleApp() = default;


		/** @brief Destroy the application instance. */
		virtual ~ConsoleApp() = default;
		/** @} */


		/** @{ */
		ConsoleApp(const ConsoleApp&) = delete;
		ConsoleApp& operator=(const ConsoleApp&) = delete;
		/** @} */


		/** @{ */
		ConsoleApp(ConsoleApp&&) = delete;
		ConsoleApp& operator=(ConsoleApp&&) = delete;
		/** @} */


		/** @{ */
		/** @brief Register a new named option to search for on the command line. */
		OptionBuilder addOption(std::string_view name) {
			return OptionBuilder(instance(), name);
		}


		/** @brief Looks up the specified option on the command line and returns its value. */
		template <typename T>
		const T& option(std::string_view name, const T& value = T()) {
			if(!m_instance) {
				return value;
			}

			const auto it = m_instance->parsed.find(std::string(name));
			if(it != m_instance->parsed.cend()) {
				if(!it->second.empty()) {
					return it->second.as<T>();
				}
			}

			return value;
		}


		/** @brief Check if the specified option is present. */
		bool option(std::string_view name) const {
			if(m_instance) {
				return m_instance->parsed.contains(std::string(name));
			}
			return false;
		}


		/** @brief Prints out the information about available options. */
		void dumpOptions(std::ostream& out = std::cout) const {
			if(m_instance) {
				out << m_instance->named;
			}
		}
		/** @} */


		/** @{ */
		/** @brief Run the application if it has not been run already. */
		int exec(int argc, gsl::czstring argv[]) noexcept;
		/** @} */
		

	protected:
		/** @{ */
		/** @brief Called on application startup. */
		virtual void onStartup() { }


		/** @brief Main application code. */
		virtual void onRun() { }


		/** @brief Called on application shutdown. */
		virtual void onShutdown() { }
		/** @} */


		/** @{ */
		/** @brief Called when some error occurs. */
		virtual void onError();
		/** @} */


	private:
		/** @{ */
		struct AppInstance {
			boost::program_options::positional_options_description positional;
			boost::program_options::options_description named = { "Options" };
			boost::program_options::variables_map parsed;

			int exitCode = 0;
			bool firstRun = true;
		};
		/** @} */


		/** @{ */
		AppInstance& instance();
		/** @} */


		/** @{ */
		std::unique_ptr<AppInstance> m_instance;
		/** @} */
	};
}

#endif