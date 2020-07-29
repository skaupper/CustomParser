# automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
    include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
    include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2 CONAN_PKG::spdlog)
target_link_libraries(catch_main PRIVATE project_warnings project_options)

add_executable(
    parser_tests
    utils/buffered_input.cpp
    parser/terminals/sign.cpp
    parser/terminals/digit.cpp
    parser/terminals/arith_op.cpp
    parser/non-terminals/integer.cpp
    parser/non-terminals/arith_expr.cpp)
target_link_libraries(
    parser_tests
    PRIVATE project_warnings
            project_options
            parser_lib
            catch_main)
target_include_directories(parser_tests PRIVATE . ../src)

catch_discover_tests(
    parser_tests
    TEST_PREFIX
    "unittests."
    EXTRA_ARGS
    -s
    --reporter=xml
    --out=tests.xml)
