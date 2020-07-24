#ifndef SK_LIB_TEST_UTILS_H
#define SK_LIB_TEST_UTILS_H

#include <catch2/catch.hpp>

#include "sk/parser.h"

using namespace sk::parser;
using namespace sk::parser::types;
using namespace std::literals::string_view_literals;


[[maybe_unused]] static bool is_good(const ParseResult &res) { return std::holds_alternative<GoodParseResult>(res); }

[[maybe_unused]] static GoodParseResult get_good(const ParseResult &res) {
    REQUIRE(is_good(res));
    return std::get<GoodParseResult>(res);
}

template<typename Parser>
ParseResult call_parser(Parser &&p, const std::string_view s) {
    std::vector<ParseStackEntry> bt;
    return p(s, bt);
}


#endif
