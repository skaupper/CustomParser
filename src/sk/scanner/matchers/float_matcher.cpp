#include "sk/scanner/matchers/float_matcher.h"

#include <spdlog/spdlog.h>


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    StepResult float_matcher::step(int c) {

        switch (state) {
        case State::INIT:
            if (c >= '0' && c <= '9') {
                spdlog::debug("float_matcher::step: PRE_POINT, {}", c);
                state = State::PRE_POINT;
                return StepResult::MATCH;
            } else if (c == '.') {
                spdlog::debug("float_matcher::step: POINT_NO_PRE, {}", c);
                state = State::POINT_NO_PRE;
                return StepResult::NO_MATCH;
            }
            break;

        case State::PRE_POINT:
            if (c >= '0' && c <= '9') {
                spdlog::debug("float_matcher::step: PRE_POINT, {}", c);
                return StepResult::MATCH;
            } else if (c == '.') {
                spdlog::debug("float_matcher::step: POINT, {}", c);
                state = State::POINT;
                return StepResult::MATCH;
            } else if (c == 'f' || c == 'F') {
                spdlog::debug("float_matcher::step: SUFFIX, {}", c);
                state = State::SUFFIX;
                return StepResult::MATCH;
            }
            break;

        case State::POINT_NO_PRE:
            if (c >= '0' && c <= '9') {
                spdlog::debug("float_matcher::step: POST_POINT, {}", c);
                state = State::POST_POINT;
                return StepResult::MATCH;
            }
            break;

        case State::POINT:
            if (c >= '0' && c <= '9') {
                spdlog::debug("float_matcher::step: POST_POINT, {}", c);
                state = State::POST_POINT;
                return StepResult::MATCH;
            } else if (c == 'f' || c == 'F') {
                spdlog::debug("float_matcher::step: SUFFIX, {}", c);
                state = State::SUFFIX;
                return StepResult::MATCH;
            }
            break;

        case State::POST_POINT:
            if (c >= '0' && c <= '9') {
                spdlog::debug("float_matcher::step: POST_POINT, {}", c);
                return StepResult::MATCH;
            } else if (c == 'f' || c == 'F') {
                spdlog::debug("float_matcher::step: SUFFIX, {}", c);
                state = State::SUFFIX;
                return StepResult::MATCH;
            }
            break;

        case State::SUFFIX:
        case State::ERROR:
            break;
        }

        spdlog::debug("NumberMatcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

}  // namespace sk::scanner::matchers
