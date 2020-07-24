#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Digit symbols.", "[parser][parser.terminals][parser.terminals.digit]") {
    // Obviously wrong
    REQUIRE(!is_good(call_parser(parse_digit, "* asdfw"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "/--"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "a+++"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "\"0aa"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "O\n0aa"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "A"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "B"sv)));
    REQUIRE(!is_good(call_parser(parse_digit, "-1"sv)));

    // Basic tests without remainder
    REQUIRE(is_good(call_parser(parse_digit, "0"sv)));
    REQUIRE(is_good(call_parser(parse_digit, "1"sv)));
    REQUIRE(is_good(call_parser(parse_digit, "3"sv)));
    REQUIRE(is_good(call_parser(parse_digit, "5"sv)));
    REQUIRE(is_good(call_parser(parse_digit, "7"sv)));
    REQUIRE(is_good(call_parser(parse_digit, "9"sv)));

    // Signs with remainder and advanced checks
    auto res {get_good(call_parser(parse_digit, "1234"sv))};
    REQUIRE(res.result == "1");
    REQUIRE(res.remaining == "234");
    REQUIRE(res.token == Token::DIGIT);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(call_parser(parse_digit, "8abcde"sv));
    REQUIRE(res.result == "8");
    REQUIRE(res.remaining == "abcde");
    REQUIRE(res.token == Token::DIGIT);
    REQUIRE(std::size(res.children) == 0);
}
