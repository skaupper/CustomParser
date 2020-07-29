#ifndef SK_LIB_SCANNER_H
#define SK_LIB_SCANNER_H

#include "concepts.h"
#include "types.h"

#include "sk/utils/buffered_input.h"


namespace sk::scanner {
    using namespace sk::utils;
    using namespace concepts;


    template<ScanMatcher... Matchers>
    class scanner final {
        std::tuple<Matchers...> matchers_;

    public:
        scanner() : matchers_ {Matchers {}...} {}
        explicit scanner(Matchers... matchers) : matchers_ {matchers...} {}

        ScanResult scan(std::istream &in_stream);

    private:
        std::optional<Symbol> get_next_symbol(buffered_input &input);
    };

}  // namespace sk::scanner


#include "impl/scanner.txx"


#endif
