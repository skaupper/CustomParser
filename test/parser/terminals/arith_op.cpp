#include <catch2/catch.hpp>

#include "utils/parser.h"


TEST_CASE("Arithmetic operator symbols.", "[parser][parser.terminals][parser.terminals.arith_op]") {
    // Obviously wrong
    REQUIRE(!is_good(parse_arith_op("^* asdfw")));
    REQUIRE(!is_good(parse_arith_op("$/--")));
    REQUIRE(!is_good(parse_arith_op("(a+++")));
    REQUIRE(!is_good(parse_arith_op("0aa")));
    REQUIRE(!is_good(parse_arith_op("abc\n0aa")));

    // Basic signs without remainder
    REQUIRE(is_good(parse_arith_op("+")));
    REQUIRE(is_good(parse_arith_op("-")));
    REQUIRE(is_good(parse_arith_op("*")));
    REQUIRE(is_good(parse_arith_op("/")));
    REQUIRE(is_good(parse_arith_op("**")));

    // Signs with remainder and advanced checks
    auto res {get_good(parse_arith_op("***"))};
    REQUIRE(res.result == "**");
    REQUIRE(res.remaining == "*");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(parse_arith_op("+++"));
    REQUIRE(res.result == "+");
    REQUIRE(res.remaining == "++");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(parse_arith_op("* **"));
    REQUIRE(res.result == "*");
    REQUIRE(res.remaining == " **");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);

    res = get_good(parse_arith_op("-1234"));
    REQUIRE(res.result == "-");
    REQUIRE(res.remaining == "1234");
    REQUIRE(res.token == Token::ARITH_OP);
    REQUIRE(std::size(res.children) == 0);
}
