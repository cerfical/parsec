#include <parsec/parsec.hpp>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <filesystem>
#include <fstream>

class ParsecOptions {
public:
	/** @{ */
	friend std::ostream& operator<<(std::ostream& out, const ParsecOptions& ops) {
		return out << ops.options;
	}
	/** @} */

	/** @{ */
	ParsecOptions(int argc, gsl::czstring argv[])
	 : options("Options") {
		FillOptions();
		ParseCmdLine(argc, argv);
	}
	/** @} */

	/** @{ */
	const std::string& GetInputFile() const {
		return vars["input-file"].as<std::string>();
	}

	bool HasInputFile() const {
		return !GetInputFile().empty();
	}
	/** @} */

	/** @{ */
	const std::string& GetOutputFile() const {
		return vars["output-file"].as<std::string>();
	}

	bool HasOutputFile() const {
		return !GetOutputFile().empty();
	}
	/** @} */

	/** @{ */
	bool VersionRequested() const {
		return vars.contains("version");
	}

	bool HelpRequested() const {
		return vars.contains("help");
	}
	/** @} */

private:
	/** @{ */
	void FillOptions() {
		using namespace boost::program_options;

		options.add_options()
			("version", "print version information")
			("help", "produce help message");

		options_description config;
		config.add_options()
			("output-file,o", value<std::string>()->default_value(""), "output file")
			("input-file,i", value<std::string>()->default_value(""), "input file");
		options.add(config);
	}

	void ParseCmdLine(int argc, gsl::czstring argv[]) {
		using namespace boost::program_options;

		store(command_line_parser(argc, argv)
			.positional(positional_options_description()
				.add("input-file", 1)
				.add("output-file", 1)
			)
			.options(options)
			.run(),
		vars);
	}
	/** @} */

	boost::program_options::options_description options;
	boost::program_options::variables_map vars;
};

class ParsecApp {
public:
	/** @{ */
	explicit ParsecApp(const ParsecOptions& ops)
	 : options(ops)
	{ }
	/** @} */

	/** @{ */
	int Run() {
		if(options.VersionRequested()) {
			return DisplayVersion();
		}
		if(options.HelpRequested() || !options.HasInputFile()) {
			return DisplayHelp();
		}
		if(!OpenFiles()) {
			return 1;
		}
		return CompileFile();
	}
	/** @} */

private:
	/** @{ */
	int DisplayHelp() const {
		std::cout << "Usage:\n";
		std::cout << "  parsec <input-file> [<output-file>]\n";
		std::cout << "  parsec [options]\n\n";
		std::cout << options << '\n';
		return 0;
	}
	/** @} */

	/** @{ */
	int DisplayVersion() const {
		std::cout << "parsec version " << PARSEC_VERSION  << ": " << PARSEC_DESCRIPTION << '\n';
		return 0;
	}
	/** @} */

	/** @{ */
	bool OpenOutputFile() {
		output.open(options.HasOutputFile() ? options.GetOutputFile() : 
			std::filesystem::path(options.GetInputFile()).replace_extension(".hpp")
		);

		if(!output.is_open()) {
			std::cerr << "fatal error: could not open the output file\n";
			return false;
		}
		return true;
	}

	bool OpenInputFile() {
		input.open(options.GetInputFile(), std::ios::binary | std::ios::in);
		if(!input.is_open()) {
			std::cerr << "fatal error: could not open the input file\n";
			return false;
		}
		return true;
	}

	bool OpenFiles() {
		return OpenInputFile() && OpenOutputFile();
	}
	/** @} */

	/** @{ */
	int CompileFile() {
		parsec::BnfParser parser(&input);
		try {
			const auto grammar = parser.Parse();
			parsec::LexCppGenerator(&grammar.GetTokens(), &output)
				.Generate();
		} catch(const parsec::ParseError& err) {
			PrintParseError(err);
			return 1;
		}
		return 0;
	}
	/** @} */

	/** @{ */
	std::string ReadLine(const parsec::SourceLocation& loc) {
		if(loc.IsEmpty()) {
			return "";
		}

		// save the current input position and then update the input pointer
		const auto inputPos = input.tellg();
		input.seekg(loc.GetStartPos());
		
		// read the line of text represented by the location
		std::string line;
		std::getline(input, line);
		input.seekg(inputPos);

		return line;
	}

	void PrintParseError(const parsec::ParseError& err) noexcept {
		const auto& loc = err.location();
		// retrieve the location text if any
		const auto line = boost::algorithm::trim_right_copy(ReadLine(loc));
		const auto trimmedLine = boost::algorithm::trim_left_copy(line);

		// print out the location information if available
		std::cerr << options.GetInputFile() << ':';
		if(!loc.IsEmpty()) {
			std::cerr << loc.GetLineNo() + 1 << ':' << loc.GetColumnNo() + 1 << ':';
		}

		// print out the error message
		std::cerr << " error: " << err.what();
		if(!trimmedLine.empty()) {
			std::cerr << ':';
		}
		std::cerr << '\n';

		// print an optional message footer highlighting the location in the source code
		if(!trimmedLine.empty()) {
			// construct a label to visually mark the location
			const auto label = std::string(loc.GetColumnCount(), loc.GetColumnCount() != 1 ? '~' : '^');
			// construct a string of spaces to move the label to the proper place
			const auto spaces = std::string(loc.GetColumnNo() + trimmedLine.size() - line.size(), ' ');

			std::cerr << "    " << trimmedLine << '\n';
			std::cerr << "    " << spaces + label << '\n';
		}
	}
	/** @} */

	const ParsecOptions& options;
	
	std::ifstream input;
	std::ofstream output;
};

int main(int argc, gsl::czstring argv[]) noexcept {
	try {
		const auto options = ParsecOptions(argc, argv);
		return ParsecApp(options).Run();
	} catch(const std::exception& err) {
		std::cerr << "fatal error: " << err.what() << '\n';
		return 1;
	}
}