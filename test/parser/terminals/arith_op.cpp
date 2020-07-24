#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Arithmetic operator symbols.", "[parser][parser.terminals][parser.terminals.arith_op]") {
    // Obviously wrong
    REQUIRE(!is_good(call_parser(parse_arith_op, "^* asdfw"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_op, "$/--"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_op, "(a+++"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_op, "0aa"sv)));
    REQUIRE(!is_good(call_parser(parse_arith_op, "abc\n0aa"sv)));

    // Basic signs without remainder
    REQUIRE(is_good(call_parser(parse_arith_op, "+"sv)));
    REQUIRE(is_good(call_parser(parse_arith_op, "-"sv)));
    REQUIRE(is_good(call_parser(parse_arith_op, "*"sv)));
    REQUIRE(is_good(call_parser(parse_arith_op, "/"sv)));
    REQUIRE(is_good(call_parser(parse_arith_op, "**"sv)));

    // Signs with remainder and advanced checks
    auto res {get_good(call_parser(parse_arith_op, "***"sv))};
    REQUIRE(res.result == "**");
    REQUIRE(res.remaining == "*");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(call_parser(parse_arith_op, "+++"sv));
    REQUIRE(res.result == "+");
    REQUIRE(res.remaining == "++");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(call_parser(parse_arith_op, "* **"sv));
    REQUIRE(res.result == "*");
    REQUIRE(res.remaining == " **");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(call_parser(parse_arith_op, "-1234"sv));
    REQUIRE(res.result == "-");
    REQUIRE(res.remaining == "1234");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);
}
