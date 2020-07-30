#ifndef SK_LIB_SCANNER_FLOAT_MATCHER_H
#define SK_LIB_SCANNER_FLOAT_MATCHER_H

#include "sk/scanner/token_map.h"
#include "sk/scanner/types.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner;
    using namespace sk::scanner::types;


    class float_matcher final {
        enum class State { INIT, PRE_POINT, POINT, POINT_NO_PRE, POST_POINT, SUFFIX, ERROR } state;
        TokenId token_ {0};

    public:
        TokenId get_token() const { return token_; }
        void reset() { state = State::INIT; }

    public:
        void init(token_map &);
        StepResult step(int c);
    };

}  // namespace sk::scanner::matchers


#endif
