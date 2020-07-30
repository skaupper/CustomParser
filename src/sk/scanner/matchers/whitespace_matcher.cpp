#include "sk/scanner/matchers/whitespace_matcher.h"
#include "sk/scanner/logger.h"
#include "sk/scanner/scanner.h"


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
    using namespace sk::scanner;
    using namespace sk::scanner::types;


    void whitespace_matcher::init(token_map &tokenMap) { token_ = tokenMap.add_token("WHITESPACE"); }

    StepResult whitespace_matcher::step(int c) {
        switch (state) {
        case State::INIT:
        case State::WS:
            if (is_whitespace(c)) {
                get_logger()->debug("whitespace_matcher::step: WS, {}", c);
                state = State::WS;
                return StepResult::MATCH;
            }
            break;

        case State::ERROR:
            break;
        }

        get_logger()->debug("whitespace_matcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

}  // namespace sk::scanner::matchers
