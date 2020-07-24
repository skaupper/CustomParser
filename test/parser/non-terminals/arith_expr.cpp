#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Arithmetic expression symbols.", "[parser][parser.non-terminals][parser.non-terminals.arith_expr]") {
    // Obviously wrong
    REQUIRE(!is_good(call_parser(parse_arith_expr, "1324* asdfw"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "/--"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "12345***1347"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "12345 **1347"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "a+++"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "bfeaa"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "%%§$\n0aa"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "+12345"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "+12345//1"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_expr, "+123+++45"sv)));

    // Basic tests without remainder
    REQUIRE(is_good(call_parser(parse_arith_expr, "+123++45"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "+12345--01354"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "+12345**+01354"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "987654/789"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "+987654*-0"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "-987654%123"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "0+0"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "1*1"sv)));
    REQUIRE(is_good(call_parser(parse_arith_expr, "0/0"sv)));

    // Arithmethic expressions with remainder and advanced checks
    auto res {get_good(call_parser(parse_arith_expr, "+12345**+01354***134"sv))};
    REQUIRE(res.result == "+12345**+01354");
    REQUIRE(res.remaining == "***134");
    REQUIRE(res.token == Token::ARITH_EXPR);
    REQUIRE(std::size(res.children) == 3);

    REQUIRE(res.children[0].token == Token::RVALUE);
    REQUIRE(res.children[1].token == Token::ARITH_OP);
    REQUIRE(res.children[2].token == Token::RVALUE);

    REQUIRE(res.children[0].result == "+12345");
    REQUIRE(res.children[1].result == "**");
    REQUIRE(res.children[2].result == "+01354");
}
