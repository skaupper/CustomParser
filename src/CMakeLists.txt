add_library(
    parser_lib OBJECT
    parser.cpp
    sk/scanner/logger.cpp
    sk/scanner/token_map.cpp
    sk/scanner/output_operators.cpp
    sk/scanner/matchers/float_matcher.cpp
    sk/scanner/matchers/number_matcher.cpp
    sk/scanner/matchers/whitespace_matcher.cpp
    sk/utils/logger.cpp
    sk/utils/buffered_input.cpp)
target_link_libraries(parser_lib PUBLIC project_options project_warnings CONAN_PKG::spdlog)
target_include_directories(parser_lib PUBLIC ../include)

add_executable(parser main.cpp)

target_link_libraries(parser PRIVATE parser_lib)
