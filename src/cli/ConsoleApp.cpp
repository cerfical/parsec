#include "cli/ConsoleApp.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace parsec::cli {
	ConsoleApp::AppInstance& ConsoleApp::instance() {
		if(!m_instance) {
			m_instance = std::make_unique<AppInstance>();
		}
		return *m_instance;
	}

	void ConsoleApp::onError() {
		try {
			throw;
		} catch(const std::exception& err) {
			std::cerr << "fatal error: " << err.what() << '\n';
		} catch(...) {
			std::cerr << "unknown error has occurred" << '\n';
		}
	}

	int ConsoleApp::exec(int argc, gsl::czstring argv[]) noexcept {
		if(!m_instance || m_instance->firstRun) {
			try {
				instance().firstRun = false;

				onStartup();
				
				store(po::command_line_parser(argc, argv)
					.positional(m_instance->positional)
					.options(m_instance->named)
					.run(), m_instance->parsed
				);

				onRun();
				
				onShutdown();
			} catch(...) {
				m_instance->exitCode = 1;
				onError();
			}
		}
		return m_instance->exitCode;
	}
}