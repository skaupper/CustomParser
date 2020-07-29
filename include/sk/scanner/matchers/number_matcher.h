#ifndef SK_LIB_SCANNER_NUMBER_MATCHER_H
#define SK_LIB_SCANNER_NUMBER_MATCHER_H

#include "sk/scanner/types.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    class number_matcher final {
        enum class State { INIT, NUMBER, ERROR } state;

    public:
        ScanToken get_token() const { return ScanToken::NUMBER; }
        void reset() { state = State::INIT; }

    public:
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#endif
