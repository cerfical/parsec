#include <parsec/config.hpp>
#include <parsec/parsec.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

namespace fs = std::filesystem;
namespace views = std::views;
namespace algo = boost::algorithm;
namespace po = boost::program_options;

class ParsecApp {
public:

    int exec(int argc, const char* argv[]) noexcept {
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

    bool parseCommandLine(int argc, const char* argv[]) {
        po::options_description named = { "Options" };
        named.add_options()("input-file,i", po::value<fs::path>(), "input spec file")("output-file,o", po::value<fs::path>(), "output source code file")("version", "print version information")("help", "produce help message");

        po::variables_map options;
        store(po::command_line_parser(argc, argv).positional(po::positional_options_description().add("input-file", 1).add("output-file", 1)).options(named).run(), options);


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


        inputPath_ = options["input-file"].as<fs::path>();
        if(const auto it = options.find("output-file"); it != options.cend()) {
            outputPath_ = it->second.as<fs::path>();
        } else {
            outputPath_ = inputPath_;
            outputPath_.replace_extension("hpp");
        }
        return true;
    }

    void compileGrammar() {
        if(input_.open(inputPath_, std::ios::binary); !input_.is_open()) {
            throw std::runtime_error(std::format("failed to load the input file \"{}\"", inputPath_.generic_string()));
        }
        input_.exceptions(std::ios::badbit | std::ios::failbit);

        // perform the compilation and save result in memory
        std::ostringstream outputMem;
        parsec::Compiler(outputMem)
            .compile(input_);

        // if everything is fine, try to store the result in the actual output file
        std::ofstream outputFile(outputPath_);
        if(!outputFile.is_open()) {
            throw std::runtime_error(std::format("failed to load the output file \"{}\"", outputPath_.generic_string()));
        }
        outputFile << std::move(outputMem).str();
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

    void printParseError(const parsec::ParseError& err) {
        const auto& errLoc = err.loc();
        auto line = readInputLine(errLoc.line.offset);

        const auto srcLineSize = line.size();
        algo::trim_left(line);
        const auto leadingWsSize = srcLineSize - line.size();
        algo::trim_right(line);

        const auto marker = makeVisualMarker(line, errLoc.startCol() - leadingWsSize, errLoc.colCount);
        const auto indent = std::string(TabSize, ' ');

        std::cerr
            << inputPath_.generic_string() << ':' << errLoc << ": error: " << err.what() << '\n'
            << indent << line << '\n'
            << indent << marker << '\n';
    }


    std::string readInputLine(int linePos) {
        const auto inputPos = input_.tellg();
        input_.seekg(linePos);

        std::string line;
        if(!isInputEnd()) {
            std::getline(input_, line);
            input_.seekg(inputPos);
        }
        return line;
    }

    bool isInputEnd() {
        if(input_.peek() == std::char_traits<char>::eof()) {
            input_.clear(input_.rdstate() ^ std::ios::eofbit);
            return true;
        }
        return false;
    }


    static std::string makeVisualMarker(const std::string& line, std::size_t pos, std::size_t size) {
        std::size_t spaces = 0;
        for(const auto& ch : line | views::take(pos)) {
            // tabs need to be treated specially as they can occur in the middle of a line
            // and cause the visual marker to be placed in the wrong location
            if(ch == '\t') {
                // insert the required number of spaces to move to the next tab stop
                const auto tabDisplaySize = TabSize - (spaces % TabSize);
                spaces += tabDisplaySize;
            } else {
                spaces++;
            }
        }
        return std::string(spaces, ' ') + std::string(size, size == 1 ? '^' : '~');
    }

    static constexpr int TabSize = 8;


    std::ifstream input_;
    fs::path outputPath_;
    fs::path inputPath_;
};

int main(int argc, const char* argv[]) noexcept {
    return ParsecApp().exec(argc, argv);
}
