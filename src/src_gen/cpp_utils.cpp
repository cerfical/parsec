#include "cpp_utils.hpp"

#include <format>
#include <sstream>

namespace parsec::src_gen::cpp_utils {
	namespace {
		void genEnumOutputOperator(std::ostream& out, const fg::Symbol& name, std::span<const fg::Symbol> values) {
			out << std::format("std::ostream& operator<<(std::ostream& out, {} v) {{", name.value()) << '\n'
				<< "\t" << "switch(v) {" << '\n';

			for(const auto& value : values) {
				out << "\t\t" << std::format("case {0}::{1}: out << \"{1}\"; break;", name.value(), value.value()) << '\n';
			}
			
			out << "\t" << "}" << '\n'
				<< "\t" << "return out;" << '\n'
				<< "}" << '\n';
		}

		void genEnumValues(std::ostream& out, std::span<const fg::Symbol> values) {
			bool first = true;
			for(const auto& value : values) {
				if(!std::exchange(first, false)) {
					out << ',' << '\n';
				}
				out << "\t" << value;
			}
			out << '\n';
		}
	}

	
	std::string makeEnum(const fg::Symbol& name, std::span<const fg::Symbol> values) {
		std::ostringstream out;

		out << std::format("enum class {} {{", name.value()) << '\n';
		genEnumValues(out, values);
		out << "};" << '\n'
			<< '\n';

		genEnumOutputOperator(out, name, values);

		return out.str();
	}
}