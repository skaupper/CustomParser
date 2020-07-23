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
                goodResults.push_back(r);
                resultStrLength += std::size(r.result);
                remaining = r.remaining;
            }
        };

    }  // namespace types


    // -----------------------------------------------------------------------------------------------------------------


    //
    // Misc helpers
    //

    GoodParseResult pass(const std::string_view s, const size_t nrOfChars,
                         const std::optional<Token> token = std::nullopt) {
        return GoodParseResult {
            .token     = token,                   //
            .result    = s.substr(0, nrOfChars),  //
            .remaining = s.substr(nrOfChars)      //
        };
    }

    GoodParseResult pass(const std::string_view s, const types::ParseState &state,
                         const std::optional<Token> token = std::nullopt) {
        return GoodParseResult {
            .token     = token,                               //
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
        std::optional<BadParseResult> all_of_impl(const std::string_view s, types::ParseState &state, Parser &&p,
                                                  RemParser... remParser) {
            // Apply a parser and return an error if it fails.
            const auto res {p(s)};
            if (const auto bad {std::get_if<BadParseResult>(&res)}) {
                return *bad;
            }

            // Collect all good results
            const auto good {std::get<GoodParseResult>(res)};
            state.addResult(good);

            // Apply remaining parsers or break out of the recursion
            if constexpr (sizeof...(remParser) > 0) {
                return all_of_impl(good.remaining, state, remParser...);
            } else {
                return std::nullopt;
            }
        }


        template<typename... Parser>
        ParseResult all_of(const std::string_view s, const Token token, Parser... parser) {
            types::ParseState state;
            if (const auto bad {all_of_impl(s, state, parser...)}) {
                return *bad;
            }

            return pass(s, state, token);
        }


    }  // namespace detail


    //
    // Parser combinator operators
    //

    template<typename Parser>
    auto optional(Parser parser) {
        return [=](const std::string_view &s) {
            return detail::optional(s, parser);
        };
    }

    template<typename... Parser>
    auto one_of(Parser... parser) {
        return [=](const std::string_view &s) {
            return detail::one_of(s, parser...);
        };
    }

    template<typename... Parser>
    auto all_of(const Token token, Parser... parser) {
        return [=](const std::string_view &s) {
            return detail::all_of(s, token, parser...);
        };
    }

    template<typename Parser>
    auto repeated(Parser &&parser, const size_t min = 0, const size_t max = 0) {
        return [=](const std::string_view &s) {
            return detail::repeated(s, parser, min, max);
        };
    }

}  // namespace sk::parser::helper


#endif
