#include "sk/scanner/matchers/number_matcher.h"

#include <spdlog/spdlog.h>


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    StepResult number_matcher::step(int c) {

        switch (state) {
        case State::INIT:
        case State::NUMBER:
            if (c >= '0' && c <= '9') {
                spdlog::debug("number_matcher::step: NUMBER, {}", c);
                state = State::NUMBER;
                return StepResult::MATCH;
            }
            break;

        case State::ERROR:
            break;
        }

        spdlog::debug("number_matcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

}  // namespace sk::scanner::matchers
