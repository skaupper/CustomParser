#include "sk/utils/container_templates.h"

#include <spdlog/spdlog.h>


namespace sk::scanner {
    using namespace sk::utils;
    using namespace concepts;


    template<ScanMatcher... Matchers>
    std::optional<Symbol> scanner<Matchers...>::get_next_symbol(buffered_input &input) {

        std::optional<ScanToken> acceptedToken;
        std::stringstream acceptedSymbol;


        // Reset all matchers
        for_each_tuple(matchers_, [](auto &cat) { cat.reset(); });


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
            for_each_tuple(matchers_, [&](auto &cat) {
                const auto stepRes {cat.step(c)};
                switch (stepRes) {
                case StepResult::MATCH:
                    cont = true;

                    // Only save the first match per character, this way earlier matcher have priority over
                    // later ones.
                    if (input.buffer_size() != 0) {
                        acceptedSymbol << input.remove_string_from_buffer();
                        acceptedToken = cat.get_token();

                        if (acceptedToken == ScanToken::NONE) {
                            spdlog::error("scanner::get_next_symbol: A token value of NONE should "
                                          "never need to be touched");
                        }
                    }
                    break;

                case StepResult::NO_MATCH:
                    cont = true;
                    break;

                case StepResult::ERROR:
                    break;
                }
            });

            // Loop until all matchers return with ERROR.
        } while (cont);


        if (!acceptedToken) {
            spdlog::debug("scanner::get_next_symbol: No symbol found");
            return std::nullopt;
        }

        spdlog::debug("scanner::get_next_symbol: New symbol {}: '{}'", *acceptedToken, acceptedSymbol.str());
        return Symbol {
            .token = *acceptedToken,       //
            .str   = acceptedSymbol.str()  //
        };
    }

    template<ScanMatcher... Matchers>
    ScanResult scanner<Matchers...>::scan(std::istream &in_stream) {
        buffered_input input(in_stream);
        std::vector<Symbol> symbols;


        while (true) {
            const auto optSymbol {get_next_symbol(input)};
            if (!optSymbol) {
                if (input.eof()) {
                    return symbols;
                } else {
                    return BadScanResult {};
                }
            }

            symbols.push_back(*optSymbol);
            input.putback_buffer();
        }
    }

}  // namespace sk::scanner
