#include <boost/algorithm/string/trim.hpp>
#include <parsec/parsec.hpp>

#include <filesystem>
#include <ranges>
#include <fstream>

namespace fs = std::filesystem;
namespace algo = boost::algorithm;

namespace ranges = std::ranges;
namespace views = ranges::views;

class ParsecApp : public parsec::cli::ConsoleApp {
private:
	/** @{ */
	void onStartup() override {
		addOption("input-file,i").desc("input file")
			.typed<fs::path>()
			.positional();
		addOption("output-file,o").desc("output file")
			.typed<fs::path>()
			.positional();
		addOption("version").desc("print version information");
		addOption("help").desc("produce help message");
	}

	void onRun() override {
		if(option("version")) {
			std::cout << "parsec version " << PARSEC_VERSION << ": " << PARSEC_DESCRIPTION << '\n';
			return;
		}

		if(option("help") || !option("input-file")) {
			std::cout << "Usage:\n";
			std::cout << "  parsec <input-file> [<output-file>]\n";
			std::cout << "  parsec [options]\n\n";
			dumpOptions();
			return;
		}

		compile();
	}

	void onError() override {
		try {
			throw;
		} catch(const parsec::ParseError& e) {
			printError("error", e);
			printNotes(e);
		} catch(...) {
			ConsoleApp::onError();
		}
	}
	/** @} */


	/** @{ */
	void compile() {
		// ensure that the specified input file is open and parse it
		m_inputPath = option<fs::path>("input-file");
		m_input.open(m_inputPath, std::ios::binary | std::ios::in);
		if(!m_input.is_open()) {
			throw std::runtime_error("could not open the input file");
		}

		const auto grammar = parsec::fg::Parser().parse(m_input);
		
		
		// ensure that the specified output file is open and write it
		m_outputPath = option("output-file", m_inputPath.stem().replace_extension("hpp"));
		m_output.open(m_outputPath);
		if(!m_output.is_open()) {
			throw std::runtime_error("failed to create the output file");
		}

		parsec::gen::CppLexerGenerator(m_output)
			.compile(grammar);
	}

	void printNotes(const parsec::ParseError& e) {
		try {
			std::rethrow_if_nested(e);
		} catch(const parsec::ParseError& e) {
			printError("note", e);
			printNotes(e);
		}
	}

	void printError(const std::string& msg, const parsec::ParseError& e) {
		auto markerPos = e.loc().startCol();

		// save the current input position and then update the input pointer
		const auto inputPos = m_input.tellg();
		m_input.seekg(e.loc().linePos());

		// read the line of text represented by the location
		std::string line;
		std::getline(m_input, line);
		m_input.seekg(inputPos);
		

		// get rid of trailing and leading whitespace and move the location marker properly
		const auto lineSize = line.size();
		algo::trim_left(line);
		markerPos -= (lineSize - line.size());
		algo::trim_right(line);


		// to correctly position the marker visually, tab characters must be taken into account
		for(int i = 0; i < markerPos; i++) {
			if(line[i] == '\t') {
				markerPos += tabSize - (i % tabSize) - 1;
			}
		}


		// finally print out the error message
		std::cerr << m_inputPath.generic_string() << ':' << e.loc() << ": "
			<< msg << ": " << e.what() << '\n';

		// marker to visually highlight the location of the error
		const auto marker = std::string(e.loc().colCount(), e.loc().colCount() != 1 ? '~' : '^');
		const auto spaces = std::string(markerPos, ' ');
		const auto indent = std::string(tabSize, ' ');

		std::cerr << indent << line << '\n'
			<< indent << spaces << marker << '\n';
	}
	/** @} */


	/** @{ */
	constexpr static auto tabSize = 8;
	/** @} */
	

	/** @{ */
	std::ifstream m_input;
	std::ofstream m_output;
	/** @} */


	/** @{ */
	fs::path m_outputPath;
	fs::path m_inputPath;
	/** @} */
};

int main(int argc, gsl::czstring argv[]) noexcept {
	return ParsecApp().exec(argc, argv);
}