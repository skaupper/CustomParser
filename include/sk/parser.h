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
            RESERVED_UNKNOWN,  //
            END_OF_FILE,       //
            LETTER,            //
            SIGN,              //
            DIGIT,             //
            ARITH_OP,          //
            LPAREN,            //
            RPAREN,            //
            COMMA,             //
            UNDERSCORE,        //
            IDENTIFIER,        //
            INTEGER,           //
            PARAMS,            //
            FUNCTION_CALL,     //
            ARITH_EXPR,        //
            COMPOUND_EXPR,     //
            RVALUE,            //
            EXPR,              //
            PROGRAM            //
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


        struct ParseStackEntry {
            Token token;
            std::string_view pos;

            friend bool operator==(const ParseStackEntry &lhs, const ParseStackEntry &rhs) {
                return (lhs.token == rhs.token) && (lhs.pos == rhs.pos);
            };
        };

    }  // namespace types


    // -----------------------------------------------------------------------------------------------------------------


    //
    // Terminal parsers
    //

    types::ParseResult parse_eof(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_letter(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_digit(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_sign(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_arith_op(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_lparen(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_rparen(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_comma(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_underscore(const std::string_view s, std::vector<types::ParseStackEntry> &);


    //
    // Non-terminal parsers
    //

    types::ParseResult parse_integer(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_identifier(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_params(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_function_call(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_arith_expr(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_compound_expr(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_rvalue(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_expr(const std::string_view s, std::vector<types::ParseStackEntry> &);
    types::ParseResult parse_program(const std::string_view s, std::vector<types::ParseStackEntry> &);

}  // namespace sk::parser


//
// operator<< overloadings
//

inline std::ostream &operator<<(std::ostream &out, const sk::parser::types::Token token) {
    out << static_cast<std::underlying_type_t<decltype(token)>>(token);
    return out;
}


#endif
