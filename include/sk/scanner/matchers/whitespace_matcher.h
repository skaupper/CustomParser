#ifndef SK_LIB_SCANNER_WHITESPACE_MATCHER_H
#define SK_LIB_SCANNER_WHITESPACE_MATCHER_H

#include "sk/scanner/types.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    class whitespace_matcher final {
        enum class State { INIT, WS, ERROR } state;

    public:
        ScanToken get_token() const { return ScanToken::WHITESPACE; }
        void reset() { state = State::INIT; }

    public:
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#endif
