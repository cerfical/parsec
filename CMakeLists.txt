cmake_minimum_required(VERSION 3.23)

project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 0.6.0)


# Set MSVC runtime library to static
if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


# Optional building of documentation
option(BUILD_DOC "Build the documentation" OFF)


# Require GSL and Boost libraries to be installed 
find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)


add_subdirectory(src)
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

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/package-config.cmake.in" [[
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/parsec-targets.cmake")
]])

configure_package_config_file("${CMAKE_CURRENT_BINARY_DIR}/package-config.cmake.in" "parsec-config.cmake"
	INSTALL_DESTINATION "cmake"
)

write_basic_package_version_file("parsec-config-version.cmake" COMPATIBILITY ExactVersion)

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
