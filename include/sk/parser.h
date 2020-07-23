#ifndef SK_LIB_PARSER_H
#define SK_LIB_PARSER_H

#include <optional>
#include <ostream>
#include <string_view>
#include <variant>
#include <vector>


namespace sk::parser {

    namespace types {

        enum class Token {
            SIGN,       //
            DIGIT,      //
            INTEGER,    //
            ARITH_OP,   //
            ARITH_EXPR  //
        };

        struct GoodParseResult {
            std::optional<Token> token {};
            std::string_view result {};
            std::vector<GoodParseResult> children {};
            std::string_view remaining;
        };

        struct BadParseResult {
            std::string_view msg;
        };

        using ParseResult = std::variant<GoodParseResult, BadParseResult>;

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


//
// operator<< overloadings
//

inline std::ostream &operator<<(std::ostream &out, const sk::parser::types::Token token) {
    out << static_cast<std::underlying_type_t<decltype(token)>>(token);
    return out;
}


#endif
