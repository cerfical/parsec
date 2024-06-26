#include "util/cpp_util.hpp"

#include "core/Symbol.hpp"

#include <format>
#include <ostream>
#include <span>
#include <sstream>
#include <string>
#include <utility>

namespace parsec::cpp_util {
    namespace {
        void genEnumOutputOperator(std::ostream& out, const Symbol& enumName, std::span<const Symbol> valueNames) {
            out << std::format("std::ostream& operator<<(std::ostream& out, {} v) {{", enumName.value()) << '\n'
                << "\tswitch(v) {\n";

            for(const auto& name : valueNames) {
                out << std::format("\t\tcase {0}::{1}: out << \"{1}\"; break;", enumName.value(), name.value()) << '\n';
            }

            out << "\t\tdefault: break;\n"
                << "\t}\n"
                << "\treturn out;\n"
                << "}\n";
        }

        void genEnumValues(std::ostream& out, std::span<const Symbol> valueNames) {
            for(bool first = true; const auto& name : valueNames) {
                if(!first) {
                    out << ',';
                } else {
                    first = false;
                }
                out << '\n' << "\t" << name.value();
            }
        }
    }


    std::string makeEnum(const Symbol& enumName, std::span<const Symbol> valueNames) {
        std::ostringstream out;

        out << std::format("enum class {} {{", enumName.value());

        genEnumValues(out, valueNames);
        if(!valueNames.empty()) {
            out << '\n';
        }
        out << "};" << '\n' << '\n';

        genEnumOutputOperator(out, enumName, valueNames);

        return std::move(out).str();
    }
}
