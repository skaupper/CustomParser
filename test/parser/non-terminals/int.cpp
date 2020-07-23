#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Integer symbols.", "[parser][parser.non-terminals][parser.non-terminals.integer]") {
    // Obviously wrong
    REQUIRE(!is_good(parse_int("* asdfw")));
    REQUIRE(!is_good(parse_int("/--")));
    REQUIRE(!is_good(parse_int("a+++")));
    REQUIRE(!is_good(parse_int("bfeaa")));
    REQUIRE(!is_good(parse_int("%%§$\n0aa")));

    // Basic tests without remainder
    REQUIRE(is_good(parse_int("+12345")));
    REQUIRE(is_good(parse_int("987654")));
    REQUIRE(is_good(parse_int("-987654")));
    REQUIRE(is_good(parse_int("0")));
    REQUIRE(is_good(parse_int("-0")));
    REQUIRE(is_good(parse_int("+0")));

    // Signs with remainder and advanced checks

    // TODO: This should produce 24 tokens (one for each digit and one for the sign)
    // but there are only 3, since the tree still has a
    // separate node for the `repeated` in it, which should be flattened out
    auto res {get_good(parse_int("+12345678910111213141516 12345"))};
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

    res = get_good(parse_sign("0011.ABC"));
    REQUIRE(res.result == "0011");
    REQUIRE(res.remaining == ".ABC");
    REQUIRE(res.token == Token::INTEGER);
    REQUIRE(std::size(res.children) == 4);
    for (size_t i {0}; i < std::size(res.children); ++i) {
        REQUIRE(res.children[i].token == Token::DIGIT);
    }
}
