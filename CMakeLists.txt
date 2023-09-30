cmake_minimum_required(VERSION 3.16)
project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 0.1)

option(BUILD_DOC "Build documentation" OFF)

if(BUILD_SHARED_LIBS)
	set(PARSEC_LIB "parsec${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
	set(PARSEC_LIB "parsec${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()
set(PARSEC_APP "parsec${CMAKE_EXECUTABLE_SUFFIX}")

add_library(${PARSEC_LIB})
target_sources(${PARSEC_LIB}
	INTERFACE
		"include/parsec/parsec.hpp"
		"include/parsec/Utils.hpp"

		"include/parsec/RegExNodes.hpp"
		"include/parsec/RegExPrinter.hpp"
		"include/parsec/RegExParser.hpp"

	PRIVATE
		"src/Utils.cpp"

		"src/RegExNodes.cpp"
		"src/RegExPrinter.cpp"
		"src/RegExParser.cpp"
)

target_include_directories(${PARSEC_LIB} PRIVATE "include/parsec")
target_include_directories(${PARSEC_LIB} INTERFACE "include")

target_compile_features(${PARSEC_LIB} PUBLIC cxx_std_20)
set_target_properties(${PARSEC_LIB}
	PROPERTIES
		SUFFIX ""
)

add_executable(${PARSEC_APP} "app/main.cpp")
target_link_libraries(${PARSEC_APP} PRIVATE ${PARSEC_LIB})
set_target_properties(${PARSEC_APP}
	PROPERTIES
		SUFFIX ""
)

if(BUILD_DOC)
	add_subdirectory(docs)
endif()