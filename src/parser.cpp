#include "sk/parser.h"
#include "sk/details/parser_helper.h"
#include <cassert>

#include <spdlog/spdlog.h>


using namespace sk::parser::types;
using namespace sk::parser::helper;
using namespace std::literals::string_view_literals;


// ---------------------------------------------------------------------------------------------------------------------


namespace sk::parser {

    //
    // Terminal parsers
    //

    ParseResult parse_sign(const std::string_view s) {
        spdlog::debug("parse_sign");

        if (std::size(s) == 0) {
            return BadParseResult {
                .msg = "parse_sign: Empty string"  //
            };
        }

        char c {s[0]};
        if (c != '-' && c != '+') {
            spdlog::debug("No sign: {}; s: {}", c, s);
            return BadParseResult {
                .msg = "parse_sign: Not a digit"  //
            };
        }

        spdlog::debug("Sign: {}; s: {}", c, s);
        return GoodParseResult {
            .token     = Token::SIGN,     //
            .result    = s.substr(0, 1),  //
            .remaining = s.substr(1)      //
        };
    }

    ParseResult parse_digit(const std::string_view s) {
        spdlog::debug("parse_digit");

        if (std::size(s) == 0) {
            return BadParseResult {
                .msg = "parse_digit: Empty string"  //
            };
        }

        char c {s[0]};
        if (c < '0' || c > '9') {
            spdlog::debug("No digit: {}; s: {}", c, s);
            return BadParseResult {
                .msg = "parse_digit: Not a digit"  //
            };
        }

        spdlog::debug("Digit: {}; s: {}", c, s);
        return GoodParseResult {
            .token     = Token::DIGIT,    //
            .result    = s.substr(0, 1),  //
            .remaining = s.substr(1)      //
        };
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
                spdlog::debug("Matched op: {:2}; s: {}", op, s);
                return GoodParseResult {
                    .token     = Token::ARITH_OP,             //
                    .result    = s.substr(0, std::size(op)),  //
                    .remaining = s.substr(std::size(op))      //
                };
            }

            spdlog::debug("Failed op: {:2}; s: {}", op, s);
        }

        return BadParseResult {
            .msg = "parse_arith_ops: No matching operator"  //
        };
    }


    //
    // Non-terminal parsers
    //

    ParseResult parse_int(const std::string_view s) {
        spdlog::debug("parse_int");

        return all_of(Token::INTEGER,        //
                      optional(parse_sign),  //
                      parse_digit,           //
                      repeated(parse_digit))(s);
    }


    ParseResult parse_arith_expr(const std::string_view s) {
        spdlog::debug("parse_arith_expr");

        return all_of(Token::ARITH_EXPR,  //
                      parse_int,          //
                      parse_arith_op,     //
                      parse_int)(s);
    }

}  // namespace sk::parser
