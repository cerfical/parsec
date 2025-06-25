#include <boost/algorithm/string/trim.hpp>
#include <boost/dll.hpp>
#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

import parsec;
import parsec.config;

namespace fs = std::filesystem;
namespace algo = boost::algorithm;
namespace po = boost::program_options;
namespace dll = boost::dll;

class ParsecOptions {
public:

    ParsecOptions()
        : named_("Options") {
        // use the directory placed along the executable as the source for templates
        const auto tmplDir = (dll::program_location().parent_path() / "templates" / "").string();
        named_.add_options()                                                                                            //
            ("input-file,i", po::value<std::string>()->required(), "input spec file")                                   //
            ("output-file,o", po::value<std::string>()->default_value("<input-file>.<template>"), "output source file") //
            ("template,t", po::value<std::string>()->default_value("json"), "output template")                          //
            ("template-dir", po::value<std::string>()->default_value(tmplDir), "template directory")                    //
            ("tab-size", po::value<std::size_t>()->default_value(4), "tab display size")                                //
            ("version", "print version information")                                                                    //
            ("help", "produce help message");                                                                           //
    }


    bool parse(int argc, const char* argv[]) {
        store(
            po::command_line_parser(argc, argv)
                .positional(
                    po::positional_options_description()
                        .add("input-file", 1)
                        .add("output-file", 1)
                )
                .options(named_)
                .run(),
            options_
        );

        if(options_.contains("help") || options_.contains("version")) {
            if(argc != 2) {
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


    const std::string& inputFile() const {
        return options_["input-file"].as<std::string>();
    }

    std::string outputFile() const {
        if(const auto& out = options_["output-file"]; !out.defaulted()) {
            return out.as<std::string>();
        }
        return fs::path(inputFile()).replace_extension(templateName()).string();
    }


    const std::string& templateDir() const {
        return options_["template-dir"].as<std::string>();
    }

    const std::string& templateName() const {
        return options_["template"].as<std::string>();
    }

    std::string templatePath() const {
        return (fs::path(templateDir()) / templateName()).string() + ".tmpl";
    }


    std::size_t tabSize() const {
        return options_["tab-size"].as<std::size_t>();
    }


private:
    po::options_description named_;
    po::variables_map options_;
};


class ParsecApp {
public:

    ParsecApp(const ParsecOptions* options)
        : options_(options) {}

    bool exec() {
        input_.open(options_->inputFile(), std::ios::binary);
        if(!input_.is_open()) {
            throw std::runtime_error(std::format("failed to load the input file \"{}\"", options_->inputFile()));
        }
        compiler_.setInputSource(&input_);

        if(options_->templateName() != "json") {
            tmpl_.open(options_->templatePath());
            if(!tmpl_.is_open()) {
                throw std::runtime_error(std::format("failed to load the template file \"{}\"", options_->templatePath()));
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
        } catch(const parsec::CompileError& e) {
            dumpError(e);
            return false;
        }

        output_.open(options_->outputFile());
        if(!output_.is_open()) {
            const auto msg = std::format(
                "failed to open the output file \"{}\"",
                options_->outputFile()
            );
            throw std::runtime_error(msg);
        }
        output_ << compiled.str();
        return true;
    }

    void dumpError(const parsec::CompileError& err) {
        const auto tabSize = options_->tabSize();
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
                const auto tabDisplaySize = tabSize - (formatted.size() % tabSize);
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
        const auto indent = std::string(tabSize, ' ');

        std::cerr
            << options_->inputFile() << ':' << err.loc() << ": error: " << err.what() << '\n'
            << indent << formatted << '\n'
            << indent << spaces << marker << '\n';
    }

    std::string readInputLine(int linePos) {
        std::string line;
        input_.seekg(linePos);
        std::getline(input_, line);
        return line;
    }


    const ParsecOptions* options_ = {};
    parsec::Compiler compiler_;

    std::ifstream input_;
    std::ifstream tmpl_;
    std::ofstream output_;
};


int main(int argc, const char* argv[]) noexcept {
    try {
        if(auto options = ParsecOptions(); !options.parse(argc, argv) && ParsecApp(&options).exec()) {
            return 0;
        }
    } catch(const std::exception& e) {
        std::cerr << "fatal error: " << e.what() << '\n';
    } catch(...) {
        std::cerr << "fatal error: unknown problem was encountered" << '\n';
    }
    return 1;
}
