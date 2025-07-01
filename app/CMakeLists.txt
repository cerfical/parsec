find_package(Boost REQUIRED COMPONENTS program_options dll)


add_executable(parsec-app "main.cxx")

target_link_libraries(parsec-app
    PRIVATE parsec-lib

    PRIVATE Boost::program_options
    PRIVATE Boost::dll
    PRIVATE Boost::boost
)


target_compile_features(parsec-app PRIVATE cxx_std_20)

set_target_properties(parsec-app PROPERTIES OUTPUT_NAME parsec)
