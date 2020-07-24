#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Integer symbols.", "[parser][parser.non-terminals][parser.non-terminals.integer]") {
    // Obviously wrong
    REQUIRE(!is_good(call_parser(parse_integer, "* asdfw"sv)));
    REQUIRE(!is_good(call_parser(parse_integer, "/--"sv)));
    REQUIRE(!is_good(call_parser(parse_integer, "a+++"sv)));
    REQUIRE(!is_good(call_parser(parse_integer, "bfeaa"sv)));
    REQUIRE(!is_good(call_parser(parse_integer, "%%§$\n0aa"sv)));

    // Basic tests without remainder
    REQUIRE(is_good(call_parser(parse_integer, "+12345"sv)));
    REQUIRE(is_good(call_parser(parse_integer, "987654"sv)));
    REQUIRE(is_good(call_parser(parse_integer, "-987654"sv)));
    REQUIRE(is_good(call_parser(parse_integer, "0"sv)));
    REQUIRE(is_good(call_parser(parse_integer, "-0"sv)));
    REQUIRE(is_good(call_parser(parse_integer, "+0"sv)));

    // Signs with remainder and advanced checks
    auto res {get_good(call_parser(parse_integer, "+12345678910111213141516 12345"sv))};
    REQUIRE(res.result == "+12345678910111213141516");
    REQUIRE(res.remaining == " 12345");
    REQUIRE(res.token == Token::INTEGER);
    REQUIRE(std::size(res.children) == 24);
    for (size_t i {0}; i < std::size(res.children); ++i) {
        if (i == 0) {
            REQUIRE(res.children[i].token == Token::SIGN);
        } else {
            REQUIRE(res.children[i].token == Token::DIGIT);
        }
    }

    res = get_good(call_parser(parse_integer, "0011.ABC"sv));
    REQUIRE(res.result == "0011");
    REQUIRE(res.remaining == ".ABC");
    REQUIRE(res.token == Token::INTEGER);
    REQUIRE(std::size(res.children) == 4);
    for (size_t i {0}; i < std::size(res.children); ++i) {
        REQUIRE(res.children[i].token == Token::DIGIT);
    }
}
