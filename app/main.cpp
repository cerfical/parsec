#include <parsec/config.hpp>
#include <parsec/parsec.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;
namespace algo = boost::algorithm;
namespace po = boost::program_options;

class ParsecOptions {
public:

    ParsecOptions(int argc, const char* argv[])
        : argc_(argc), argv_(argv), named_("Options") {
        named_.add_options()                                                                   //
            ("input-file,i", po::value<fs::path>()->required(), "input spec file")             //
            ("output-file,o", po::value<fs::path>(), "output source file")                     //
            ("template,t", po::value<std::string>()->default_value("json"), "output template") //
            ("template-dir", po::value<fs::path>(), "template directory")                      //
            ("version", "print version information")                                           //
            ("help", "produce help message");                                                  //

        store(
            po::command_line_parser(argc, argv)
                .positional(po::positional_options_description()
                                .add("input-file", 1)
                                .add("output-file", 1))
                .options(named_)
                .run(),
            options_
        );
    }


    bool dumpUsageInfo() {
        if(options_.contains("help") || options_.contains("version")) {
            if(argc_ != 2) {
                throw std::runtime_error("invalid command line options");
            }

            if(options_.contains("help")) {
                std::cerr
                    << "Usage:\n"
                       "  parsec <input-file> [<output-file>]\n"
                       "  parsec [options]\n\n"
                    << named_
                    << '\n';
            } else {
                std::cerr << std::format(
                    "parsec version {}: {}\n\n",
                    parsec::Config::version(),
                    parsec::Config::description()
                );
            }
            return true;
        }
        notify(options_);
        return false;
    }


    fs::path inputFile() const {
        return options_["input-file"].as<fs::path>();
    }

    fs::path outputFile() const {
        if(const auto& out = options_["output-file"]; !out.empty()) {
            return out.as<fs::path>();
        }
        return inputFile().replace_extension(templateName());
    }


    fs::path templateDir() const {
        if(const auto& tmplDir = options_["template-dir"]; !tmplDir.empty()) {
            return tmplDir.as<fs::path>();
        }

        // use the directory placed along the executable as the source for templates
        return fs::path(argv_[0]).remove_filename() / "templates" / "";
    }

    std::string templateName() const {
        return options_["template"].as<std::string>();
    }

    fs::path templatePath() const {
        return templateDir().append(templateName()).concat(".tmpl");
    }


private:
    po::options_description named_;
    po::variables_map options_;

    int argc_ = {};
    const char** argv_ = {};
};


class ParsecApp {
public:

    ParsecApp(const ParsecOptions* options)
        : options_(options) {}

    bool exec() {
        input_.open(options_->inputFile(), std::ios::binary);
        if(!input_.is_open()) {
            throw std::runtime_error(std::format("failed to load the input file \"{}\"", options_->inputFile().string()));
        }
        input_.exceptions(std::ios::badbit | std::ios::failbit);
        compiler_.setInputSource(&input_);

        if(options_->templateName() != "json") {
            tmpl_.open(options_->templatePath(), std::ios::binary);
            if(!tmpl_.is_open()) {
                throw std::runtime_error(std::format(
                    "failed to load the template file \"{}\"",
                    options_->templatePath().string()
                ));
            }
            compiler_.setOutputTemplateSource(&tmpl_);
        }

        return compile();
    }

private:
    bool compile() {
        std::ostringstream compiled;
        compiler_.setOutputSink(&compiled);
        try {
            compiler_.compile();
        } catch(const parsec::ParseError& e) {
            dumpError(e);
            return false;
        }

        output_.open(options_->outputFile());
        if(!output_.is_open()) {
            throw std::runtime_error(std::format(
                "failed to open the output file \"{}\"",
                options_->outputFile().string()
            ));
        }
        output_ << compiled.str();
        return true;
    }

    void dumpError(const parsec::ParseError& err) {
        auto line = readInputLine(err.loc().line.offset);
        algo::trim_right(line);

        // remove leading whitespace
        const auto srcLineSize = line.size();
        algo::trim_left(line);
        const auto markerPos = err.loc().startCol() - (srcLineSize - line.size());

        std::size_t extraSpaces = 0;
        std::string formatted;

        for(std::size_t i = 0; i < line.size(); i++) {
            // tabs are treated specially as they can cause the marker to be visually misplaced
            if(line[i] == '\t') {
                const auto tabDisplaySize = TabSize - (formatted.size() % TabSize);
                formatted.append(tabDisplaySize, ' ');

                if(i < markerPos) {
                    extraSpaces += tabDisplaySize - 1;
                }
            } else {
                formatted += line[i];
            }
        }

        const auto spaces = std::string(markerPos + extraSpaces, ' ');
        const auto marker = std::string(std::max(err.loc().colCount, 1), err.loc().colCount > 1 ? '~' : '^');
        const auto indent = std::string(TabSize, ' ');

        std::cerr
            << options_->inputFile().string() << ':' << err.loc() << ": error: " << err.what() << '\n'
            << indent << formatted << '\n'
            << indent << spaces << marker << '\n';
    }

    static constexpr auto TabSize = 4;


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


    const ParsecOptions* options_ = {};
    parsec::Compiler compiler_;

    std::ifstream input_;
    std::ifstream tmpl_;
    std::ofstream output_;
};


int main(int argc, const char* argv[]) noexcept {
    try {
        auto options = ParsecOptions(argc, argv);
        if(!options.dumpUsageInfo() && ParsecApp(&options).exec()) {
            return 0;
        }
    } catch(const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << '\n';
    } catch(...) {
        std::cerr << "fatal error: unknown problem was encountered" << '\n';
    }
    return 1;
}
