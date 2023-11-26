cmake_minimum_required(VERSION 3.23)

project(parsec LANGUAGES CXX DESCRIPTION "Simple parser generator" VERSION 1.0.0)


# Optional building of documentation and examples
option(BUILD_DOC "Build the documentation" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# By default build static libs
option(BUILD_SHARED_LIBS "Enable building of shared libraries" OFF)


# Require GSL and Boost libraries to be installed 
find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)


add_subdirectory(src)
add_subdirectory(app)

if(BUILD_DOC)
	add_subdirectory(docs)
endif()

if(BUILD_EXAMPLES)
	add_subdirectory(examples/calc)
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
