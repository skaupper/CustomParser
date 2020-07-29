#ifndef SK_LIB_SCANNER_CONCEPTS_H
#define SK_LIB_SCANNER_CONCEPTS_H

#include "types.h"


namespace sk::scanner::concepts {
    using namespace sk::scanner::types;


    template<typename T>
    concept ScanMatcher = requires(T a, int c) {
        { a.step(c) }
        ->StepResult;

        { a.reset() }
        ->void;

        { a.get_token() }
        ->ScanToken
    };

}  // namespace sk::scanner::concepts

#endif
