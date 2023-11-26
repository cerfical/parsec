add_executable(parsec "main.cpp")

target_link_libraries(parsec
	PRIVATE parsec-lib
	PRIVATE Boost::program_options
	PRIVATE Boost::boost
)

target_compile_features(parsec PRIVATE cxx_std_20)
