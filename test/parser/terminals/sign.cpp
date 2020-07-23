#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Sign symbols.", "[parser][parser.terminals][parser.terminals.sign]") {
    // Obviously no signs
    REQUIRE(!is_good(parse_sign("* asdfw")));
    REQUIRE(!is_good(parse_sign("/--")));
    REQUIRE(!is_good(parse_sign("a+++")));
    REQUIRE(!is_good(parse_sign("0aa")));
    REQUIRE(!is_good(parse_sign("\n0aa")));

    // Basic tests without remainder
    REQUIRE(is_good(parse_sign("+")));
    REQUIRE(is_good(parse_sign("-")));

    // Signs with remainder and advanced checks
    auto res {get_good(parse_sign("-1234"))};
    REQUIRE(res.result == "-");
    REQUIRE(res.remaining == "1234");
    REQUIRE(res.token == Token::SIGN);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(parse_sign("+abcd5678"));
    REQUIRE(res.result == "+");
    REQUIRE(res.remaining == "abcd5678");
    REQUIRE(res.token == Token::SIGN);
    REQUIRE(std::size(res.children) == 0);
}
