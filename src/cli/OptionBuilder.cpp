#include "cli/OptionBuilder.hpp"
#include "cli/OptionStore.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/make_shared.hpp>

namespace po = boost::program_options;

namespace parsec::cli {
	OptionBuilder::OptionBuilder(OptionStore& options, std::string_view name)
		: m_value(std::make_unique<po::untyped_value>(true)) // by default the option is valueless
		, m_name(name)
		, m_options(options)
	{ }

	OptionBuilder::~OptionBuilder() {
		const auto optionDesc = boost::make_shared<po::option_description>(
			m_name.c_str(),
			m_value.release(),
			m_desc.c_str()
		);
		m_options.m_named.add(optionDesc);

		if(m_positional) {
			m_options.m_positional.add(optionDesc->long_name().c_str(), 1);
		}

		m_options.m_changed = true;
	}
}