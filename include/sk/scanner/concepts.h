#ifndef SK_LIB_SCANNER_CONCEPTS_H
#define SK_LIB_SCANNER_CONCEPTS_H

#include "token_map.h"
#include "types.h"


namespace sk::scanner::concepts {
    using namespace sk::scanner;
    using namespace sk::scanner::types;


    template<typename T>
    concept ScanMatcher = requires(T a, int c, token_map &tokenMap) {
        {T {}};

        { a.init(tokenMap) }
        ->void;

        { a.step(c) }
        ->StepResult;

        { a.reset() }
        ->void;

        { a.get_token() }
        ->TokenId
    };

}  // namespace sk::scanner::concepts

#endif
