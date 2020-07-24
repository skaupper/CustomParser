#include "sk/parser.h"
#include "sk/details/parser_helper.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>


using namespace sk::parser::types;
using namespace sk::parser::helper;
using namespace std::literals::string_view_literals;


namespace sk::parser {

    template<char CHAR, Token TOKEN = Token::RESERVED_UNKNOWN>
    ParseResult parse_char(const std::string_view s) {
        static const std::string FUNCTION_NAME {fmt::format("parse_char<'{}'>", CHAR)};
        spdlog::debug(FUNCTION_NAME);

        if (std::size(s) == 0) {
            return fail(FUNCTION_NAME + ": Empty string");
        }

        if (s[0] != CHAR) {
            spdlog::debug("Not matching: s: '{}'", s);
            return fail(FUNCTION_NAME + ": Not a digit");
        }

        spdlog::debug("Match: s: '{}'", s);
        return pass(s, 1, TOKEN);
    }


    //
    // Terminal parsers
    //

    ParseResult parse_eof(const std::string_view s) {
        spdlog::debug("parse_eof");

        if (std::size(s) != 0) {
            return fail("parse_eof: Non-empty string"sv);
        }

        return pass(s, 0, Token::END_OF_FILE);
    }


    ParseResult parse_letter(const std::string_view s) {
        spdlog::debug("parse_letter");

        if (std::size(s) == 0) {
            return fail("parse_letter: Empty string"sv);
        }

        char c {s[0]};
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) {
            spdlog::debug("No letter: s: '{}'", s);
            return fail("parse_letter: Not a letter"sv);
        }

        spdlog::debug("Letter: s: '{}'", s);
        return pass(s, 1, Token::LETTER);
    }


    ParseResult parse_digit(const std::string_view s) {
        spdlog::debug("parse_digit");

        if (std::size(s) == 0) {
            return fail("parse_digit: Empty string"sv);
        }

        char c {s[0]};
        if (c < '0' || c > '9') {
            spdlog::debug("No digit: s: '{}'", s);
            return fail("parse_digit: Not a digit"sv);
        }

        spdlog::debug("Digit: s: '{}'", s);
        return pass(s, 1, Token::DIGIT);
    }

    ParseResult parse_sign(const std::string_view s) {
        spdlog::debug("parse_sign");
        return one_of<Token::SIGN>(parse_char<'+'>,  //
                                   parse_char<'-'>)(s);
    }

    ParseResult parse_arith_op(const std::string_view s) {
        spdlog::debug("parse_arith_ops");

        constexpr std::array AVAILABLE_OPS {
            "**"sv,         //
            "%"sv,          //
            "*"sv,  "/"sv,  //
            "+"sv,  "-"sv,  //
        };

        for (const auto &op : AVAILABLE_OPS) {
            if (s.starts_with(op)) {
                spdlog::debug("Matched op: '{}'; s: '{}'", op, s);
                return pass(s, std::size(op), Token::ARITH_OP);
            }
        }

        spdlog::debug("No op matched: s: '{}'", s);
        return fail("parse_arith_op: No matching operator"sv);  //
    }

    ParseResult parse_lparen(const std::string_view s) {
        spdlog::debug("parse_lparen");
        return parse_char<'(', Token::LPAREN>(s);
    }

    ParseResult parse_rparen(const std::string_view s) {
        spdlog::debug("parse_rparen");
        return parse_char<')', Token::RPAREN>(s);
    }

    ParseResult parse_comma(const std::string_view s) {
        spdlog::debug("parse_comma");
        return parse_char<',', Token::COMMA>(s);
    }

    ParseResult parse_underscore(const std::string_view s) {
        spdlog::debug("parse_underscore");
        return parse_char<'_', Token::UNDERSCORE>(s);
    }


    //
    // Non-terminal parsers
    //

    ParseResult parse_integer(const std::string_view s) {
        spdlog::debug("parse_integer");

        return all_of<Token::INTEGER>(optional(parse_sign),  //
                                      parse_digit,           //
                                      repeated(parse_digit))(s);
    }

    ParseResult parse_identifier(const std::string_view s) {
        spdlog::debug("parse_identifier");

        return all_of<Token::IDENTIFIER>(parse_letter,                      //
                                         repeated(one_of(parse_underscore,  //
                                                         parse_letter))     //
                                         )(s);
    }

    ParseResult parse_params(const std::string_view s) {
        spdlog::debug("parse_params");

        return all_of<Token::PARAMS>(parse_rvalue,                      //
                                     repeated(all_of(parse_underscore,  //
                                                     parse_rvalue))     //
                                     )(s);
    }

    ParseResult parse_function_call(const std::string_view s) {
        spdlog::debug("parse_function_call");

        return all_of<Token::FUNCTION_CALL>(parse_identifier,  //
                                            parse_lparen,      //
                                            parse_params,      //
                                            parse_rparen)(s);
    }


    ParseResult parse_arith_expr(const std::string_view s) {
        spdlog::debug("parse_arith_expr");

        return all_of<Token::ARITH_EXPR>(parse_rvalue,    //
                                         parse_arith_op,  //
                                         parse_rvalue)(s);
    }


    ParseResult parse_compound_expr(const std::string_view s) {
        spdlog::debug("parse_compound_expr");

        return all_of<Token::COMPOUND_EXPR>(parse_lparen,      //
                                            parse_arith_expr,  //
                                            parse_rparen)(s);
    }


    ParseResult parse_rvalue(const std::string_view s) {
        spdlog::debug("parse_rvalue");

        return one_of<Token::RVALUE>(parse_integer,        //
                                     parse_function_call,  //
                                     parse_expr)(s);
    }


    ParseResult parse_expr(const std::string_view s) {
        spdlog::debug("parse_expr");

        return one_of<Token::EXPR>(parse_compound_expr,  //
                                   parse_arith_expr)(s);
    }


    ParseResult parse_program(const std::string_view s) {
        spdlog::debug("parse_program");

        return all_of<Token::PROGRAM>(parse_expr,  //
                                      parse_eof)(s);
    }

}  // namespace sk::parser
