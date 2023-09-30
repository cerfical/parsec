cmake_minimum_required(VERSION 3.16)

# Define project metadata
project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 0.5.2)

# Set MSVC runtime library to static
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Optional building of documentation
option(BUILD_DOC "Build the documentation" OFF)

# Require GSL and Boost libraries to be installed 
find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

# Write project information to a header file
configure_file("src/ProjectConfig.hpp.in" "ProjectConfig.hpp")

# Add a target for the project's library
add_library(parsec-lib)

target_sources(parsec-lib
	INTERFACE
		"include/parsec/parsec.hpp"
		"include/parsec/Utils.hpp"

		"include/parsec/BnfGrammar.hpp"

		"include/parsec/LexPattern.hpp"
		"include/parsec/LexGenerator.hpp"
		"include/parsec/LexCppGenerator.hpp"

		"include/parsec/RegExNodes.hpp"
		"include/parsec/RegExPrinter.hpp"
		"include/parsec/RegExParser.hpp"

		"include/parsec/BnfToken.hpp"
		"include/parsec/BnfLexer.hpp"
		"include/parsec/BnfParser.hpp"

	PRIVATE
		"src/Utils.cpp"

		"src/LexPattern.cpp"
		"src/LexGenerator.cpp"
		"src/LexCppGenerator.cpp"

		"src/RegExNodes.cpp"
		"src/RegExPrinter.cpp"
		"src/RegExParser.cpp"

		"src/BnfLexer.cpp"
		"src/BnfParser.cpp"
)

set_target_properties(parsec-lib
	PROPERTIES
		OUTPUT_NAME "parsec"
)

# Set interface and build-only include directories
target_include_directories(parsec-lib
	INTERFACE "include"
	PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
	PRIVATE "include/parsec"
)

# Set usage requirements
target_compile_features(parsec-lib PUBLIC cxx_std_20)
target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)

add_subdirectory(app)

# Optionally build documentation
if(BUILD_DOC)
	add_subdirectory(docs)
endif()