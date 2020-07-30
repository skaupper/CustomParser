#include "sk/utils/container_templates.h"

#include "sk/scanner/logger.h"


namespace sk::scanner::matchers {
    using namespace sk::utils;
    using namespace sk::scanner::types;
    using namespace sk::scanner::concepts;


    template<ScanMatcher... Matchers>
    void combined_matcher<Matchers...>::init(token_map &tokenMap) {
        for_each_tuple(matchers_, [&](auto &matcher) { matcher.init(tokenMap); });
    }

    template<ScanMatcher... Matchers>
    StepResult combined_matcher<Matchers...>::step(int c) {
        StepResult res {StepResult::ERROR};
        latestMatchToken_ = INVALID_TOKEN;


        bool first {true};

        for_each_tuple(matchers_, [&](auto &matcher) {
            const auto stepRes {matcher.step(c)};
            switch (stepRes) {
            case StepResult::MATCH:
                // Only the token of the first match is interesting
                if (first) {
                    first             = false;
                    latestMatchToken_ = matcher.get_token();
                    if (latestMatchToken_ == INVALID_TOKEN) {
                        sk::scanner::get_logger()->error("combined_matcher::step: A token value of NONE should "
                                                         "never need to be touched");
                    }
                }

                if (res != StepResult::MATCH) {
                    res = StepResult::MATCH;
                }
                break;

            case StepResult::NO_MATCH:
                if (res == StepResult::ERROR) {
                    res = StepResult::NO_MATCH;
                }
                break;

            case StepResult::ERROR:
                break;
            }
        });

        return res;
    }

}  // namespace sk::scanner::matchers
