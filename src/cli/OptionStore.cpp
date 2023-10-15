#include "cli/OptionStore.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace parsec::cli {
	void OptionStore::updateParsed() const {
		if(m_changed) {
			store(po::command_line_parser(m_argc, m_argv)
				.positional(m_positional)
				.options(m_named)
				.run(), m_parsed
			);
			m_changed = false;
		}
	}
}