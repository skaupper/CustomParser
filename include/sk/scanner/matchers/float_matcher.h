#ifndef SK_LIB_SCANNER_FLOAT_MATCHER_H
#define SK_LIB_SCANNER_FLOAT_MATCHER_H

#include "sk/scanner/types.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner::types;


    class float_matcher final {
        enum class State { INIT, PRE_POINT, POINT, POINT_NO_PRE, POST_POINT, SUFFIX, ERROR } state;

    public:
        ScanToken get_token() const { return ScanToken::FLOAT; }
        void reset() { state = State::INIT; }

    public:
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#endif
