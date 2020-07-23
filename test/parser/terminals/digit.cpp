#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Test case for parsing digit symbols.", "[parser][parser.terminals][parser.terminals.digit]") {
    // Obviously no signs
    REQUIRE(!is_good(parse_digit("* asdfw")));
    REQUIRE(!is_good(parse_digit("/--")));
    REQUIRE(!is_good(parse_digit("a+++")));
    REQUIRE(!is_good(parse_digit("\"0aa")));
    REQUIRE(!is_good(parse_digit("O\n0aa")));
    REQUIRE(!is_good(parse_digit("A")));
    REQUIRE(!is_good(parse_digit("B")));
    REQUIRE(!is_good(parse_digit("-1")));

    // Basic signs without remainder
    REQUIRE(is_good(parse_digit("0")));
    REQUIRE(is_good(parse_digit("1")));
    REQUIRE(is_good(parse_digit("3")));
    REQUIRE(is_good(parse_digit("5")));
    REQUIRE(is_good(parse_digit("7")));
    REQUIRE(is_good(parse_digit("9")));

    // Signs with remainder and advanced checks
    auto res {get_good(parse_digit("1234"))};
    REQUIRE(res.result == "1");
    REQUIRE(res.remaining == "234");
    REQUIRE(res.token == Token::DIGIT);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(parse_digit("8abcde"));
    REQUIRE(res.result == "8");
    REQUIRE(res.remaining == "abcde");
    REQUIRE(res.token == Token::DIGIT);
    REQUIRE(std::size(res.children) == 0);
}
