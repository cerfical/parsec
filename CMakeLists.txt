cmake_minimum_required(VERSION 3.23)

project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 0.5.3)

# Set MSVC runtime library to static
if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Optional building of documentation
option(BUILD_DOC "Build the documentation" OFF)

# Require GSL and Boost libraries to be installed 
find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

# Write project information to a header file
configure_file("cmake/parsec-config.hpp.in" "parsec-config.hpp")

add_library(parsec-lib STATIC
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

target_sources(parsec-lib
	PUBLIC FILE_SET HEADERS FILES
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

	BASE_DIRS
		"include"
)

set_target_properties(parsec-lib
	PROPERTIES
		OUTPUT_NAME "parsec"
)

target_include_directories(parsec-lib
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
	PRIVATE "include/parsec"
)

target_compile_features(parsec-lib PUBLIC cxx_std_20)
target_link_libraries(parsec-lib
	PUBLIC Microsoft.GSL::GSL
	PRIVATE Boost::boost
)

add_subdirectory(app)

if(BUILD_DOC)
	add_subdirectory(docs)
endif()

# Export and install targets 
install(TARGETS parsec EXPORT parsec-targets RUNTIME DESTINATION ".")
install(EXPORT parsec-targets NAMESPACE parsec:: DESTINATION "cmake")

export(EXPORT parsec-targets)

# Configure and install package config and version files
include(CMakePackageConfigHelpers)

configure_package_config_file("cmake/package-config.cmake.in" "parsec-config.cmake"
	INSTALL_DESTINATION "cmake"
)
write_basic_package_version_file("parsec-config-version.cmake"
	COMPATIBILITY ExactVersion
)

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/parsec-config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/parsec-config-version.cmake"
	DESTINATION "cmake"
)

# Enable packaging with CPack
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_GENERATOR "ZIP")
include(CPack)