#ifndef SK_LIB_PARSER_H
#define SK_LIB_PARSER_H

#include <optional>
#include <string_view>
#include <variant>
#include <vector>


namespace sk::parser {

    namespace types {

        enum class Token { UNKNOWN, SIGN, DIGIT, INTEGER, ARITH_OP, ARITH_EXPR };

        struct GoodParseResult {
            Token token {Token::UNKNOWN};
            std::string_view result {};
            std::vector<GoodParseResult> children {};
            std::string_view remaining;
        };

        struct BadParseResult {
            std::string_view msg;
        };

        using ParseResult = std::variant<GoodParseResult, BadParseResult>;

        using parse_fun_t = ParseResult(const std::string_view);

    }  // namespace types


    // -----------------------------------------------------------------------------------------------------------------


    //
    // Terminal parsers
    //

    types::ParseResult parse_sign(const std::string_view s);
    types::ParseResult parse_digit(const std::string_view s);
    types::ParseResult parse_arith_op(const std::string_view s);


    //
    // Non-terminal parsers
    //

    types::ParseResult parse_int(const std::string_view s);
    types::ParseResult parse_arith_expr(const std::string_view s);

}  // namespace sk::parser

#endif
