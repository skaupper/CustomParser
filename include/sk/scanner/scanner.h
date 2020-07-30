#ifndef SK_LIB_SCANNER_H
#define SK_LIB_SCANNER_H

#include "concepts.h"
#include "token_map.h"
#include "types.h"

#include "sk/utils/buffered_input.h"


namespace sk::scanner {
    using namespace sk::utils;
    using namespace concepts;


    template<ScanMatcher... Matchers>
    class scanner final {
        std::tuple<Matchers...> matchers_;
        token_map tokenMap_;

        size_t line_;
        size_t col_;

    public:
        scanner() : matchers_ {Matchers {}...} { init_matchers(); }
        explicit scanner(Matchers... matchers) : matchers_ {matchers...} { init_matchers(); }

        ScanResult scan(std::istream &in_stream);
        token_map get_token_map() const;

    private:
        void init_matchers();
        std::optional<Symbol> get_next_symbol(buffered_input &input);
    };

}  // namespace sk::scanner


#include "impl/scanner.txx"


#endif
