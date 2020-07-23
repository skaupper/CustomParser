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

            return GoodParseResult {
                .remaining = s  //
            };
        }


        //
        // repeated
        //

        template<typename Parser>
        ParseResult repeated(const std::string_view s, Parser &&p, size_t min, size_t max) {
            types::ParseState state;
            state.remaining = s;

            std::string_view current_view {s};
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
                    return BadParseResult {
                        .msg = "repeated: Exceeded maximum"  //
                    };
                }

                state.addResult(std::get<GoodParseResult>(res));
            }

            // If the parser applied less often than the minimum required amount,
            // fail with an error.
            if (counter < min) {
                return BadParseResult {
                    .msg = "repeated: Subceeded minimum"  //
                };
            }

            return GoodParseResult {
                .result    = s.substr(0, state.resultStrLength),  //
                .children  = state.goodResults,                   //
                .remaining = state.remaining                      //
            };
        }


        //
        // one_of
        //

        template<typename Parser, typename... RemParser>
        ParseResult one_of(const std::string_view s, Parser &&p, RemParser... remParser) {
            // Apply
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
            const auto res {p(s)};
            if (const auto bad {std::get_if<BadParseResult>(&res)}) {
                return *bad;
            }

            const auto good {std::get<GoodParseResult>(res)};
            state.addResult(good);

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

            return GoodParseResult {
                .token     = token,                               //
                .result    = s.substr(0, state.resultStrLength),  //
                .children  = state.goodResults,                   //
                .remaining = state.remaining                      //
            };
        }


    }  // namespace detail


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
    auto repeated(Parser &&parser, size_t min = 0, size_t max = 0) {
        return [=](const std::string_view &s) {
            return detail::repeated(s, parser, min, max);
        };
    }

}  // namespace sk::parser::helper


#endif
