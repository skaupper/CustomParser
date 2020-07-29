#ifndef SK_LIB_SCANNER_COMBINED_MATCHER_H
#define SK_LIB_SCANNER_COMBINED_MATCHER_H

#include "sk/scanner/concepts.h"
#include "sk/scanner/types.h"
#include "sk/utils/container_templates.h"


namespace sk::scanner::matchers {
    using namespace sk::utils;
    using namespace sk::scanner::types;
    using namespace sk::scanner::concepts;


    template<ScanMatcher... Matchers>
    class combined_matcher final {
        std::tuple<Matchers...> matchers_;
        ScanToken latestMatchToken_;

    public:
        combined_matcher() : matchers_ {Matchers {}...} {}
        explicit combined_matcher(Matchers... matchers) : matchers_ {matchers...} {}


        ScanToken get_token() const { return latestMatchToken_; }
        void reset() {
            for_each_tuple(matchers_, [](auto &cat) { cat.reset(); });
        }

    public:
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#include "impl/combined_matcher.txx"


#endif
