cmake_minimum_required(VERSION 3.16)

# Add a target for the project's executable
add_executable(parsec-app "main.cpp")
set_target_properties(parsec-app
	PROPERTIES
		OUTPUT_NAME "parsec"
)

# Link to the executable its dependencies
target_link_libraries(parsec-app PRIVATE parsec-lib
	Microsoft.GSL::GSL
	Boost::program_options
)

# Set the C++ standard required to compile the executable
target_compile_features(parsec-app PRIVATE cxx_std_20)
