#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Integer symbols.", "[parser][parser.non-terminals][parser.non-terminals.integer]") {
    // Obviously wrong
    REQUIRE(!is_good(parse_integer("* asdfw")));
    REQUIRE(!is_good(parse_integer("/--")));
    REQUIRE(!is_good(parse_integer("a+++")));
    REQUIRE(!is_good(parse_integer("bfeaa")));
    REQUIRE(!is_good(parse_integer("%%§$\n0aa")));

    // Basic tests without remainder
    REQUIRE(is_good(parse_integer("+12345")));
    REQUIRE(is_good(parse_integer("987654")));
    REQUIRE(is_good(parse_integer("-987654")));
    REQUIRE(is_good(parse_integer("0")));
    REQUIRE(is_good(parse_integer("-0")));
    REQUIRE(is_good(parse_integer("+0")));

    // Signs with remainder and advanced checks
    auto res {get_good(parse_integer("+12345678910111213141516 12345"))};
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

    res = get_good(parse_integer("0011.ABC"));
    REQUIRE(res.result == "0011");
    REQUIRE(res.remaining == ".ABC");
    REQUIRE(res.token == Token::INTEGER);
    REQUIRE(std::size(res.children) == 4);
    for (size_t i {0}; i < std::size(res.children); ++i) {
        REQUIRE(res.children[i].token == Token::DIGIT);
    }
}
