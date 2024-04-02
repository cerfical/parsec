#include <parsec/parsec.hpp>
#include <parsec/config.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include <gsl/gsl>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;
namespace algo = boost::algorithm;
namespace po = boost::program_options;

namespace ranges = std::ranges;
namespace views = std::views;

class ParsecApp {
public:
	int exec(int argc, gsl::czstring argv[]) noexcept {
		int exitCode = 0;
		try {
			if(parseCommandLine(argc, argv)) {
				compileGrammar();
			}
		} catch(...) {
			exitCode = 1;
			dumpCurrentException();
		}
		return exitCode;
	}

private:
	/** @{ */
	bool parseCommandLine(int argc, gsl::czstring argv[]) {
		po::options_description named = { "Options" };
		named.add_options()
			("input-file,i", po::value<fs::path>(), "input grammar file")
			("output-file,o", po::value<fs::path>(), "output source code file")
			("version", "print version information")
			("help", "produce help message");

		po::variables_map options;
		store(po::command_line_parser(argc, argv)
			.positional(po::positional_options_description()
				.add("input-file", 1)
				.add("output-file", 1)
			)
			.options(named)
			.run(), options
		);


		const bool helpRequested = options.contains("help") || options.empty();
		const bool versionRequested = options.contains("version");
		
		if(helpRequested || versionRequested) {
			if(options.size() != 1) {
				throw std::runtime_error("invalid command line options");
			}

			if(helpRequested) {
				std::cout
					<< "Usage:" << '\n'
					<< "  parsec <input-file> [<output-file>]" << '\n'
					<< "  parsec [options]" << '\n'
					<< '\n'
					<< named << '\n';
			} else if(versionRequested) {
				std::cout
					<< std::format("parsec version {}: {}", PARSEC_VERSION, PARSEC_DESCRIPTION) << '\n'
					<< '\n';
			}

			return false;
		}


		m_inputPath = options["input-file"].as<fs::path>();
		if(const auto it = options.find("output-file"); it != options.cend()) {
			m_outputPath = it->second.as<fs::path>();
		} else {
			m_outputPath = m_inputPath;
			m_outputPath.replace_extension("hpp");
		}
		return true;
	}

	void dumpCurrentException() {
		try {
			throw;
		} catch(const parsec::ParseError& e) {
			printParseError(e);
		} catch(const std::exception& e) {
			std::cerr << "fatal error: " << e.what() << '\n';
		} catch(...) {
			std::cerr << "unknown error has occurred" << '\n';
		}
	}
	
	void compileGrammar() {
		if(m_input.open(m_inputPath, std::ios::binary); !m_input.is_open()) {
			throw std::runtime_error(std::format("failed to load the input file '{}'",
				m_inputPath.generic_string()
			));
		}
		m_input.exceptions(std::ios::badbit | std::ios::failbit);

		if(m_output.open(m_outputPath); !m_output.is_open()) {
			throw std::runtime_error(std::format("failed to load the output file '{}'",
				m_outputPath.generic_string()
			));
		}

		parsec::Compiler(m_output)
			.compile(m_input);
	}

	void printLoc(std::ostream& out, const parsec::SourceLoc& loc) {
		out << loc.lineNo() + 1 << ':' << loc.startCol() + 1;
		if(loc.colCount()) {
			out << '-' << (loc.endCol() - 1) + 1;
		}
	}

	void printParseError(const parsec::ParseError& e) {
		const auto line = readInputLineAt(e.loc().linePos());
		const auto marker = makeVisualMarker(line, e.loc().startCol(), e.loc().colCount());
		const auto indent = std::string(tabSize, ' ');

		std::cerr << m_inputPath.generic_string() << ':';
		printLoc(std::cerr, e.loc());
		std::cerr << ": error: " << e.what() << '\n'
			<< indent << algo::trim_copy(line) << '\n'
			<< indent << marker << '\n';
	}
	/** @} */


	/** @{ */
	std::string makeVisualMarker(const std::string& line, int startCol, int colCount) {
		const auto leadingWhitespace = line.size() - algo::trim_left_copy(line).size();
		const auto marker = std::string(colCount, colCount == 1 ? '^' : '~');

		std::string spaces;
		for(const auto i : views::iota(0) | views::take(startCol - leadingWhitespace)) {
			// tabs need to be treated specially as they can occur in the middle of a line
			// and cause the visual marker to be placed in the wrong location
			if(line[i] == '\t') {
				// insert the required number of spaces to move to the next tab stop
				const auto tabDisplaySize = tabSize - i % tabSize;
				spaces.append(tabDisplaySize, ' ');
			} else {
				spaces.append(1, ' ');
			}
		}

		return spaces + marker;
	}

	std::string readInputLineAt(int linePos) {
		const auto inputPos = m_input.tellg();
		m_input.seekg(linePos);

		std::string line;
		if(!checkForInputEof()) {
			std::getline(m_input, line);
			m_input.seekg(inputPos);
		}
		return line;
	}

	bool checkForInputEof() {
		if(m_input.peek() == std::char_traits<char>::eof()) {
			m_input.clear(m_input.rdstate() ^ std::ios::eofbit);
			return true;
		}
		return false;
	}
	/** @} */


	static constexpr int tabSize = 8;

	std::ifstream m_input;
	std::ofstream m_output;

	fs::path m_outputPath;
	fs::path m_inputPath;
};

int main(int argc, gsl::czstring argv[]) noexcept {
	return ParsecApp().exec(argc, argv);
}