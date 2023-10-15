#ifndef PARSEC_CONSOLE_APP_HEADER
#define PARSEC_CONSOLE_APP_HEADER

#include "OptionStore.hpp"
#include <gsl/gsl>

namespace parsec::cli {
	/**
	 * @brief Facilitates writing console applications, making it easier, for example, to handle command line arguments.
	 */
	class ConsoleApp {
	public:
		/** @{ */
		/** @brief Initialize a new application instance. */
		ConsoleApp(int argc, gsl::czstring argv[])
			: m_options(argc, argv)
		{ }


		/** @brief Destroy the application instance. */
		~ConsoleApp() = default;
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
		/** @brief Read access to the configuration options. */
		const OptionStore* options() const noexcept {
			return &m_options;
		}


		/** @brief Get the application's configuration options. */
		OptionStore* options() noexcept {
			return &m_options;
		}


		/** @brief Run the application if it has not already been run. */
		int exec() noexcept;
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
		virtual void onError() { }
		/** @} */


	private:
		OptionStore m_options;
		int m_exitCode = 0;
		bool m_firstRun = true;
	};
}

#endif