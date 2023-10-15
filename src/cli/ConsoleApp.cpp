#include "cli/ConsoleApp.hpp"

namespace parsec::cli {
	int ConsoleApp::exec() noexcept {
		if(m_firstRun) {
			try {
				onStartup();
				onRun();
				onShutdown();
			} catch(...) {
				onError();
				m_exitCode = 1;
			}
			m_firstRun = false;
		}
		return m_exitCode;
	}
}