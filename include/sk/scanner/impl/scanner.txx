#include "sk/utils/container_templates.h"

#include "sk/scanner/logger.h"


namespace sk::scanner {
    using namespace sk::utils;
    using namespace sk::scanner::types;
    using namespace concepts;


    template<ScanMatcher... Matchers>
    token_map scanner<Matchers...>::get_token_map() const {
        return tokenMap_;
    }

    template<ScanMatcher... Matchers>
    void scanner<Matchers...>::init_matchers() {
        for_each_tuple(matchers_, [&](auto &matcher) { matcher.init(tokenMap_); });
    }


    template<ScanMatcher... Matchers>
    std::optional<Symbol> scanner<Matchers...>::get_next_symbol(buffered_input &input) {

        std::optional<TokenId> acceptedToken;
        std::stringstream acceptedSymbol;


        // Reset all matchers
        for_each_tuple(matchers_, [](auto &matcher) { matcher.reset(); });


        bool cont;
        do {
            cont = false;

            // Get the next character
            const auto optChar {input.get_char()};
            if (!optChar) {
                return std::nullopt;
            }
            const auto c {*optChar};


            // Apply the matchers one after another.
            // If any of these return a MATCH save the symbol temporarily and continue until all matcher return
            // an ERROR. At this point take the most recent MATCH (that's the longest valid symbol at this
            // point).
            for_each_tuple(matchers_, [&](auto &matcher) {
                const auto stepRes {matcher.step(c)};
                switch (stepRes) {
                case StepResult::MATCH:
                    cont = true;

                    // Only save the first match per character, this way earlier matcher have priority over
                    // later ones.
                    if (input.buffer_size() != 0) {
                        const auto optString {input.remove_string_from_buffer()};
                        if (!optString) {
                            get_logger()->error("scanner::get_next_symbol: Failed to get buffered string.");
                            return;
                        }

                        acceptedSymbol << *optString;
                        acceptedToken = matcher.get_token();

                        if (acceptedToken == INVALID_TOKEN) {
                            get_logger()->error("scanner::get_next_symbol: A token value of NONE should "
                                                "never need to be touched");
                        }
                    }
                    break;

                case StepResult::NO_MATCH:
                    // As long as we are not in an ERROR state keep on searching for an accepted state
                    cont = true;
                    break;

                case StepResult::ERROR:
                    break;
                }
            });

            // Loop until all matchers return with ERROR.
        } while (cont);


        // No token found
        if (!acceptedToken) {
            get_logger()->debug("scanner::get_next_symbol: No symbol found");
            return std::nullopt;
        }

        // Resolve the received token ID
        const auto optToken {tokenMap_.resolve_token_id(*acceptedToken)};
        if (!optToken) {
            get_logger()->error("scanner::get_next_symbol: Could not resolve token ID '{}'", *acceptedToken);
            return std::nullopt;
        }

        get_logger()->debug("scanner::get_next_symbol: New symbol {}: '{}'", *optToken, acceptedSymbol.str());


        return Symbol {
            .tokenId  = *acceptedToken,       //
            .tokenStr = *optToken,            //
            .str      = acceptedSymbol.str()  //
        };
    }

    template<ScanMatcher... Matchers>
    ScanResult scanner<Matchers...>::scan(std::istream &in_stream) {
        buffered_input input(in_stream);
        std::vector<Symbol> symbols;

        // Reset state
        line_ = 1;
        col_  = 1;


        while (true) {
            // Try to find the next symbol
            const auto optSymbol {get_next_symbol(input)};

            // If none is found we are either at the EOF (and return all symbols found),
            // or the input did not match any known symbol (in which case we return an error)
            if (!optSymbol) {
                if (input.eof() && input.buffer_size() == 0) {
                    return symbols;
                } else {
                    BadScanResult result {.line = line_, .col = col_, .str = {}};
                    const auto optString {input.remove_string_from_buffer()};
                    if (optString) {
                        result.str = *optString;
                    } else {
                        get_logger()->error("scanner::scan: Failed to get unmatched symbol.");
                    }
                    return result;
                }
            }

            // Store symbol and prepare the buffer for the next run
            symbols.push_back(*optSymbol);
            input.putback_buffer();


            // Advance position counters for better error handling
            std::for_each(std::cbegin(optSymbol->str), std::cend(optSymbol->str), [&](const char c) {
                if (c == '\n') {
                    ++line_;
                    col_ = 1;
                } else {
                    ++col_;
                }
            });
        }
    }

}  // namespace sk::scanner
