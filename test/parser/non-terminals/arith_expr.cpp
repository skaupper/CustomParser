#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Arithmetic expression symbols.", "[parser][parser.non-terminals][parser.non-terminals.arith_expr]") {
    // Obviously wrong
    REQUIRE(!is_good(parse_arith_expr("1324* asdfw")));
    REQUIRE(!is_good(parse_arith_expr("/--")));
    REQUIRE(!is_good(parse_arith_expr("12345***1347")));
    REQUIRE(!is_good(parse_arith_expr("12345 **1347")));
    REQUIRE(!is_good(parse_arith_expr("a+++")));
    REQUIRE(!is_good(parse_arith_expr("bfeaa")));
    REQUIRE(!is_good(parse_arith_expr("%%§$\n0aa")));
    REQUIRE(!is_good(parse_arith_expr("+12345")));
    REQUIRE(!is_good(parse_arith_expr("+12345//1")));
    REQUIRE(!is_good(parse_arith_expr("+123+++45")));

    // Basic tests without remainder
    REQUIRE(is_good(parse_arith_expr("+123++45")));
    REQUIRE(is_good(parse_arith_expr("+12345--01354")));
    REQUIRE(is_good(parse_arith_expr("+12345**+01354")));
    REQUIRE(is_good(parse_arith_expr("987654/789")));
    REQUIRE(is_good(parse_arith_expr("+987654*-0")));
    REQUIRE(is_good(parse_arith_expr("-987654%123")));
    REQUIRE(is_good(parse_arith_expr("0+0")));
    REQUIRE(is_good(parse_arith_expr("1*1")));
    REQUIRE(is_good(parse_arith_expr("0/0")));

    // Arithmethic expressions with remainder and advanced checks
    auto res {get_good(parse_arith_expr("+12345**+01354***134"))};
    REQUIRE(res.result == "+12345**+01354");
    REQUIRE(res.remaining == "***134");
    REQUIRE(res.token == Token::ARITH_EXPR);
    REQUIRE(std::size(res.children) == 3);

    REQUIRE(res.children[0].token == Token::INTEGER);
    REQUIRE(res.children[1].token == Token::ARITH_OP);
    REQUIRE(res.children[2].token == Token::INTEGER);

    REQUIRE(res.children[0].result == "+12345");
    REQUIRE(res.children[1].result == "**");
    REQUIRE(res.children[2].result == "+01354");
}
