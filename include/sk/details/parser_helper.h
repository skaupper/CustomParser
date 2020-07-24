#ifndef SK_LIB_PARSER_HELPER_H
#define SK_LIB_PARSER_HELPER_H

#include "sk/parser.h"


namespace sk::parser::helper {

    using namespace sk::parser::types;


    namespace types {

        struct ParseState {
            std::vector<GoodParseResult> goodResults;
            size_t resultStrLength {0};
            std::string_view remaining;

            void addResult(const GoodParseResult &r) {
                if (!r.token) {
                    std::for_each(std::cbegin(r.children), std::cend(r.children), [&](const auto &e) { addResult(e); });
                } else {
                    goodResults.push_back(r);
                    resultStrLength += std::size(r.result);
                    remaining = r.remaining;
                }
            }
        };

    }  // namespace types


    // -----------------------------------------------------------------------------------------------------------------


    //
    // Misc helpers
    //

    std::optional<Token> to_opt_token(const Token token) {
        if (token == Token::RESERVED_UNKNOWN) {
            return std::nullopt;
        }
        return token;
    }

    GoodParseResult pass(const std::string_view s, const size_t nrOfChars,
                         const Token token = Token::RESERVED_UNKNOWN) {
        return GoodParseResult {
            .token     = to_opt_token(token),     //
            .result    = s.substr(0, nrOfChars),  //
            .remaining = s.substr(nrOfChars)      //
        };
    }

    GoodParseResult pass(const std::string_view s, const types::ParseState &state,
                         const Token token = Token::RESERVED_UNKNOWN) {
        return GoodParseResult {
            .token     = to_opt_token(token),                 //
            .result    = s.substr(0, state.resultStrLength),  //
            .children  = state.goodResults,                   //
            .remaining = state.remaining                      //
        };
    }

    BadParseResult fail(const std::string_view msg) {
        return BadParseResult {
            .msg = msg  //
        };
    }


    // -----------------------------------------------------------------------------------------------------------------


    namespace detail {

        //
        // optional
        //

        template<typename Parser>
        ParseResult optional(const std::string_view s, Parser &&p) {
            const auto res {p(s)};
            if (const auto good {std::get_if<GoodParseResult>(&res)}) {
                return *good;
            }

            // A failed parsing attempt does not change the string
            return pass(s, 0);
        }


        //
        // repeated
        //

        template<typename Parser>
        ParseResult repeated(const std::string_view s, Parser &&p, const size_t min, const size_t max) {
            types::ParseState state;
            state.remaining = s;

            size_t counter {0};

            // Apply the given parser as often as possible
            while (true) {
                const auto res {p(s.substr(state.resultStrLength))};
                if (const auto bad {std::get_if<BadParseResult>(&res)}) {
                    break;
                }

                // If the parser applied more often than the maximum allowed amount,
                // fail with an error.
                // If max == 0, there is no upper limit.
                counter++;
                if (counter > max && max != 0) {
                    return fail("repeated: Exceeded maximum");
                }

                state.addResult(std::get<GoodParseResult>(res));
            }

            // If the parser applied less often than the minimum required amount,
            // fail with an error.
            if (counter < min) {
                return fail("repeated: Subceeded minimum");
            }

            return pass(s, state);
        }


        //
        // one_of
        //

        template<typename Parser, typename... RemParser>
        ParseResult one_of(const std::string_view s, Parser &&p, RemParser... remParser) {
            // Apply one parser after another until at least one returned with a good result.
            // Otherwise an error is returned.
            const auto res {p(s)};
            if (const auto good {std::get_if<GoodParseResult>(&res)}) {
                return *good;
            }

            if constexpr (sizeof...(remParser) > 0) {
                return one_of(s, remParser...);
            } else {
                return std::get<BadParseResult>(res);
            }
        }


        //
        // all
        //

        template<typename Parser, typename... RemParser>
        std::optional<BadParseResult> all_of_impl(types::ParseState &state, Parser &&p, RemParser... remParser) {
            // Apply a parser and return an error if it fails.
            const auto res {p(state.remaining)};
            if (const auto bad {std::get_if<BadParseResult>(&res)}) {
                return *bad;
            }

            // Collect all good results
            state.addResult(std::get<GoodParseResult>(res));

            // Apply remaining parsers or break out of the recursion
            if constexpr (sizeof...(remParser) > 0) {
                return all_of_impl(state, remParser...);
            } else {
                return std::nullopt;
            }
        }


        template<typename... Parser>
        ParseResult all_of(const std::string_view s, Parser... parser) {
            types::ParseState state;
            state.remaining = s;

            if (const auto bad {all_of_impl(state, parser...)}) {
                return *bad;
            }

            return pass(s, state);
        }

        template<Token TOKEN>
        ParseResult change_token(ParseResult &&res) {
            if constexpr (TOKEN == Token::RESERVED_UNKNOWN) {
                return res;
            } else if (auto good {std::get_if<GoodParseResult>(&res)}) {
                good->token = TOKEN;
                return res;
            } else {
                return res;
            }
        }

    }  // namespace detail


    //
    // Parser combinator operators
    //

    template<Token TOKEN = Token::RESERVED_UNKNOWN, typename Parser>
    auto optional(Parser parser) {
        return [=](const std::string_view &s) {
            return detail::change_token<TOKEN>(detail::optional(s, parser));
        };
    }

    template<Token TOKEN = Token::RESERVED_UNKNOWN, typename... Parser>
    auto one_of(Parser... parser) {
        return [=](const std::string_view &s) {
            return detail::change_token<TOKEN>(detail::one_of(s, parser...));
        };
    }

    template<Token TOKEN = Token::RESERVED_UNKNOWN, typename... Parser>
    auto all_of(Parser... parser) {
        return [=](const std::string_view &s) {
            return detail::change_token<TOKEN>(detail::all_of(s, parser...));
        };
    }

    template<Token TOKEN = Token::RESERVED_UNKNOWN, typename Parser>
    auto repeated(Parser &&parser, const size_t min = 0, const size_t max = 0) {
        return [=](const std::string_view &s) {
            return detail::change_token<TOKEN>(detail::repeated(s, parser, min, max));
        };
    }

}  // namespace sk::parser::helper


#endif
