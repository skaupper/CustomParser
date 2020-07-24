#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Sign symbols.", "[parser][parser.terminals][parser.terminals.sign]") {
    // Obviously no signs
    REQUIRE(!is_good(call_parser(parse_sign, "* asdfw"sv)));
    REQUIRE(!is_good(call_parser(parse_sign, "/--"sv)));
    REQUIRE(!is_good(call_parser(parse_sign, "a+++"sv)));
    REQUIRE(!is_good(call_parser(parse_sign, "0aa"sv)));
    REQUIRE(!is_good(call_parser(parse_sign, "\n0aa"sv)));

    // Basic tests without remainder
    REQUIRE(is_good(call_parser(parse_sign, "+"sv)));
    REQUIRE(is_good(call_parser(parse_sign, "-"sv)));

    // Signs with remainder and advanced checks
    auto res {get_good(call_parser(parse_sign, "-1234"sv))};
    REQUIRE(res.result == "-");
    REQUIRE(res.remaining == "1234");
    REQUIRE(res.token == Token::SIGN);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(call_parser(parse_sign, "+abcd5678"sv));
    REQUIRE(res.result == "+");
    REQUIRE(res.remaining == "abcd5678");
    REQUIRE(res.token == Token::SIGN);
    REQUIRE(std::size(res.children) == 0);
}
