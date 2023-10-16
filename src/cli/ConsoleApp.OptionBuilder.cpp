#include "cli/ConsoleApp.hpp"
#include <boost/make_shared.hpp>

namespace po = boost::program_options;

namespace parsec::cli {
	ConsoleApp::OptionBuilder::OptionBuilder(AppInstance& app, std::string_view name)
		: m_value(std::make_unique<po::untyped_value>(true)) // by default the option is valueless
		, m_name(name)
		, m_app(app)
	{ }

	ConsoleApp::OptionBuilder::~OptionBuilder() {
		const auto optionDesc = boost::make_shared<po::option_description>(
			m_name.c_str(),
			m_value.release(),
			m_desc.c_str()
		);
		m_app.named.add(optionDesc);

		if(m_positional) {
			m_app.positional.add(optionDesc->long_name().c_str(), 1);
		}
	}
}