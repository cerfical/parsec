cmake_minimum_required(VERSION 3.16)

# Define project metadata 
project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 0.2)
# Allow for optional building of documentation
option(BUILD_DOC "Build the documentation." OFF)

# Find Microsoft's Guidelines Support Library
find_package(Microsoft.GSL REQUIRED)

# Define appropriate target names for the project's executable and library
if(BUILD_SHARED_LIBS)
	set(PARSEC_LIB "parsec${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
	set(PARSEC_LIB "parsec${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()
set(PARSEC_APP "parsec${CMAKE_EXECUTABLE_SUFFIX}")

# Add a target for the project's library
add_library(${PARSEC_LIB})
# List files that make up the library
target_sources(${PARSEC_LIB}
	INTERFACE
		"include/parsec/parsec.hpp"
		"include/parsec/Utils.hpp"

		"include/parsec/RegExNodes.hpp"
		"include/parsec/RegExPrinter.hpp"
		"include/parsec/RegExParser.hpp"

		"include/parsec/Token.hpp"
		"include/parsec/Lexer.hpp"

	PRIVATE
		"src/Utils.cpp"

		"src/RegExNodes.cpp"
		"src/RegExPrinter.cpp"
		"src/RegExParser.cpp"

		"src/Lexer.cpp"
)

# Set include directories for library code and code using the library
target_include_directories(${PARSEC_LIB} PRIVATE "include/parsec")
target_include_directories(${PARSEC_LIB} INTERFACE "include")

# Set compile options and link dependencies required to compile the library
target_compile_features(${PARSEC_LIB} PUBLIC cxx_std_20)
target_link_libraries(${PARSEC_LIB} PUBLIC Microsoft.GSL::GSL)
# Clear the output suffix, as it is already part of the target name
set_target_properties(${PARSEC_LIB}
	PROPERTIES
		SUFFIX ""
)

# Add a target for the project's executable
add_executable(${PARSEC_APP} "app/main.cpp")
# Add the project's library to the link dependencies of executable
target_link_libraries(${PARSEC_APP} PRIVATE ${PARSEC_LIB})
# Clear the output suffix, as it is already part of the target name
set_target_properties(${PARSEC_APP}
	PROPERTIES
		SUFFIX ""
)

# Optionally build documentation
if(BUILD_DOC)
	add_subdirectory(docs)
endif()