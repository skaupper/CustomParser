#ifndef SK_LIB_SCANNER_COMBINED_MATCHER_H
#define SK_LIB_SCANNER_COMBINED_MATCHER_H

#include "sk/scanner/concepts.h"
#include "sk/scanner/token_map.h"
#include "sk/scanner/types.h"
#include "sk/utils/container_templates.h"


namespace sk::scanner::matchers {
    using namespace sk::utils;
    using namespace sk::scanner;
    using namespace sk::scanner::types;
    using namespace sk::scanner::concepts;


    template<ScanMatcher... Matchers>
    class combined_matcher final {
        std::tuple<Matchers...> matchers_;
        TokenId latestMatchToken_ {0};

    public:
        combined_matcher() : matchers_ {Matchers {}...} {}
        explicit combined_matcher(Matchers... matchers) : matchers_ {matchers...} {}


        TokenId get_token() const { return latestMatchToken_; }
        void reset() {
            for_each_tuple(matchers_, [](auto &matcher) { matcher.reset(); });
        }

    public:
        void init(token_map &);
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#include "impl/combined_matcher.txx"


#endif
