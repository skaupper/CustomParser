#include "sk/scanner/matchers/number_matcher.h"
#include "sk/scanner/logger.h"
#include "sk/scanner/scanner.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner;
    using namespace sk::scanner::types;


    void number_matcher::init(token_map &tokenMap) { token_ = tokenMap.add_token("NUMBER"); }

    StepResult number_matcher::step(int c) {

        switch (state) {
        case State::INIT:
        case State::NUMBER:
            if (c >= '0' && c <= '9') {
                get_logger()->debug("number_matcher::step: NUMBER, {}", c);
                state = State::NUMBER;
                return StepResult::MATCH;
            }
            break;

        case State::ERROR:
            break;
        }

        get_logger()->debug("number_matcher::step: ERROR, {}", c);
        state = State::ERROR;
        return StepResult::ERROR;
    }

}  // namespace sk::scanner::matchers
