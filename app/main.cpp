#include <parsec/parsec.hpp>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <filesystem>
#include <fstream>

class ParsecOptions {
public:
	/** @{ */
	friend std::ostream& operator<<(std::ostream& out, const ParsecOptions& ops) {
		return out << ops.m_options;
	}
	/** @} */

	/** @{ */
	ParsecOptions(int argc, gsl::czstring argv[])
		: m_options("Options") {
		fillOptions();
		parseCmdLine(argc, argv);
	}
	/** @} */

	/** @{ */
	const std::string& inputFile() const {
		return m_vars["input-file"].as<std::string>();
	}
	bool hasInputFile() const {
		return !inputFile().empty();
	}
	/** @} */

	/** @{ */
	const std::string& outputFile() const {
		return m_vars["output-file"].as<std::string>();
	}
	bool hasOutputFile() const {
		return !outputFile().empty();
	}
	/** @} */

	/** @{ */
	bool version() const {
		return m_vars.contains("version");
	}
	bool help() const {
		return m_vars.contains("help");
	}
	/** @} */

private:
	/** @{ */
	void fillOptions() {
		using namespace boost::program_options;

		m_options.add_options()
			("version", "print version information")
			("help", "produce help message");

		options_description config;
		config.add_options()
			("output-file,o", value<std::string>()->default_value(""), "output file")
			("input-file,i", value<std::string>()->default_value(""), "input file");
		m_options.add(config);
	}
	void parseCmdLine(int argc, gsl::czstring argv[]) {
		using namespace boost::program_options;

		store(command_line_parser(argc, argv)
			.positional(positional_options_description()
				.add("input-file", 1)
				.add("output-file", 1)
			)
			.options(m_options)
			.run(),
		m_vars);
	}
	/** @} */

	boost::program_options::options_description m_options;
	boost::program_options::variables_map m_vars;
};

class ParsecApp {
public:
	/** @{ */
	explicit ParsecApp(const ParsecOptions& options)
		: m_options(options)
	{ }
	/** @} */

	/** @{ */
	int exec() {
		if(m_options.version()) {
			return displayVersion();
		}
		if(m_options.help() || !m_options.hasInputFile()) {
			return displayHelp();
		}
		if(!openFiles()) {
			return 1;
		}
		return compileFile();
	}
	/** @} */

private:
	/** @{ */
	int displayHelp() const {
		std::cout << "Usage:\n";
		std::cout << "  parsec <input-file> [<output-file>]\n";
		std::cout << "  parsec [options]\n\n";
		std::cout << m_options << '\n';
		return 0;
	}
	int displayVersion() const {
		std::cout << "parsec version " << PARSEC_VERSION  << ": " << PARSEC_DESCRIPTION << '\n';
		return 0;
	}
	/** @} */

	/** @{ */
	bool openOutputFile() {
		m_output.open(m_options.hasOutputFile() ? m_options.outputFile() :
			std::filesystem::path(m_options.inputFile()).replace_extension(".hpp")
		);

		if(!m_output.is_open()) {
			std::cerr << "fatal error: could not open the output file\n";
			return false;
		}
		return true;
	}
	bool openInputFile() {
		m_input.open(m_options.inputFile(), std::ios::binary | std::ios::in);
		if(!m_input.is_open()) {
			std::cerr << "fatal error: could not open the input file\n";
			return false;
		}
		return true;
	}
	bool openFiles() {
		return openInputFile() && openOutputFile();
	}
	/** @} */

	/** @{ */
	int compileFile() {
		parsec::fg::Parser parser;
		try {
			const auto grammar = parser.parse(m_input);
			parsec::gen::CppLexerGenerator(m_output)
				.compile(grammar);
		} catch(const parsec::ParseError& err) {
			printParseError(err);
			return 1;
		}
		return 0;
	}
	/** @} */

	/** @{ */
	std::string readLine(const parsec::SourceLoc& loc) {
		// save the current input position and then update the input pointer
		const auto inputPos = m_input.tellg();
		m_input.seekg(loc.pos());
		
		// read the line of text represented by the location
		std::string line;
		std::getline(m_input, line);
		m_input.seekg(inputPos);

		return line;
	}
	void printParseError(const parsec::ParseError& err) noexcept {
		const auto& loc = err.location();
		// retrieve the location text if any
		const auto line = boost::algorithm::trim_right_copy(readLine(loc));
		const auto trimmedLine = boost::algorithm::trim_left_copy(line);

		// print out the location information if available
		std::cerr << m_options.inputFile() << ':' << loc.line() + 1 << ':' << loc.startCol() + 1 << ':';

		// print out the error message
		std::cerr << " error: " << err.what();
		if(!trimmedLine.empty()) {
			std::cerr << ':';
		}
		std::cerr << '\n';

		// print an optional message footer highlighting the location in the source code
		if(!trimmedLine.empty()) {
			// construct a label to visually mark the location
			const auto label = std::string(loc.colCount(), loc.colCount() != 1 ? '~' : '^');
			// construct a string of spaces to move the label to the proper place
			const auto spaces = std::string(loc.startCol() + trimmedLine.size() - line.size(), ' ');

			std::cerr << "    " << trimmedLine << '\n';
			std::cerr << "    " << spaces + label << '\n';
		}
	}
	/** @} */

	const ParsecOptions& m_options;
	
	std::ifstream m_input;
	std::ofstream m_output;
};

int main(int argc, gsl::czstring argv[]) noexcept {
	try {
		const auto options = ParsecOptions(argc, argv);
		return ParsecApp(options).exec();
	} catch(const std::exception& err) {
		std::cerr << "fatal error: " << err.what() << '\n';
		return 1;
	}
}