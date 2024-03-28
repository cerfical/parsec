#include "src_gen/cpp_utils.hpp"

#include <format>
#include <sstream>

namespace parsec::src_gen::cpp_utils {
	namespace {
		void genEnumOutputOperator(std::ostream& out, const fg::Symbol& enumName, std::span<const fg::Symbol> valueNames) {
			out << std::format("std::ostream& operator<<(std::ostream& out, {} v) {{", enumName.value()) << '\n';
			out << "\t" << "switch(v) {" << '\n';

			for(const auto& name : valueNames) {
				out << "\t\t" << std::format("case {0}::{1}: out << \"{1}\"; break;", enumName.value(), name.value()) << '\n';
			}
			out << "\t\t" << "default: break;" << '\n';
			
			out << "\t" << "}" << '\n';
			out << "\t" << "return out;" << '\n';
			out << "}" << '\n';
		}


		void genEnumValues(std::ostream& out, std::span<const fg::Symbol> valueNames) {
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

	
	std::string makeEnum(const fg::Symbol& enumName, std::span<const fg::Symbol> valueNames) {
		std::ostringstream out;

		out << std::format("enum class {} {{", enumName.value());
		
		genEnumValues(out, valueNames);
		if(!valueNames.empty()) {
			out << '\n';
		}

		out << "};" << '\n';
		out << '\n';

		genEnumOutputOperator(out, enumName, valueNames);

		return std::move(out).str();
	}
}