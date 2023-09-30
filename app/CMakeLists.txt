add_executable(parsec "main.cpp")

target_link_libraries(parsec PRIVATE parsec-lib
	Microsoft.GSL::GSL
	Boost::boost
	Boost::program_options
)

target_compile_features(parsec PRIVATE cxx_std_20)
