cmake_minimum_required(VERSION 3.16)

# Add a target for the project's executable
add_executable(parsec-app
	"PrintUtils.hpp"
	"PrintUtils.cpp"

	"main.cpp"
)

# Link to the executable its dependencies
target_link_libraries(parsec-app PRIVATE
	parsec-lib Boost::boost Microsoft.GSL::GSL
)
# Set the C++ standard required to compile the executable
target_compile_features(parsec-app PRIVATE cxx_std_20)
