#include "sk/utils/container_templates.h"

#include <spdlog/spdlog.h>


namespace sk::scanner::matchers {
    using namespace sk::utils;
    using namespace sk::scanner::types;
    using namespace sk::scanner::concepts;


    template<ScanMatcher... Matchers>
    StepResult combined_matcher<Matchers...>::step(int c) {
        StepResult res {StepResult::ERROR};
        latestMatchToken_ = ScanToken::NONE;


        bool first {true};

        for_each_tuple(matchers_, [&](auto &cat) {
            const auto stepRes {cat.step(c)};
            switch (stepRes) {
            case StepResult::MATCH:
                // Only the token of the first match is interesting
                if (first) {
                    first             = false;
                    latestMatchToken_ = cat.get_token();
                    if (latestMatchToken_ == ScanToken::NONE) {
                        spdlog::error("combined_matcher::step: A token value of NONE should "
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
