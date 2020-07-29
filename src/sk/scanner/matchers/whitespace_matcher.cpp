#include "sk/scanner/matchers/whitespace_matcher.h"

#include <spdlog/spdlog.h>


static bool is_whitespace(int c) {
    switch (c) {
    case '\t':
    case '\r':
    case '\n':
    case ' ':
        return true;
    default:
        return false;
    }
}


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    StepResult whitespace_matcher::step(int c) {
        switch (state) {
        case State::INIT:
        case State::WS:
            if (is_whitespace(c)) {
                spdlog::debug("whitespace_matcher::step: WS, {}", c);
                state = State::WS;
                return StepResult::MATCH;
            }
            break;

        case State::ERROR:
            break;
        }

        spdlog::debug("whitespace_matcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

}  // namespace sk::scanner::matchers
