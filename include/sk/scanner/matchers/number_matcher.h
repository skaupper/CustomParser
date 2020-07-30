#ifndef SK_LIB_SCANNER_NUMBER_MATCHER_H
#define SK_LIB_SCANNER_NUMBER_MATCHER_H

#include "sk/scanner/token_map.h"
#include "sk/scanner/types.h"


namespace sk::scanner::matchers {
    using namespace sk::scanner;
    using namespace sk::scanner::types;


    class number_matcher final {
        enum class State { INIT, NUMBER, ERROR } state;
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
