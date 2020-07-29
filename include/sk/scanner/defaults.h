#ifndef SK_LIB_SCANNER_DEFAULTS_H
#define SK_LIB_SCANNER_DEFAULTS_H

#include "scanner.h"

#include "matchers/combined_matcher.h"
#include "matchers/float_matcher.h"
#include "matchers/number_matcher.h"
#include "matchers/whitespace_matcher.h"


namespace sk::scanner {
    using namespace matchers;

    using default_matcher_t = combined_matcher<number_matcher, float_matcher, whitespace_matcher>;
    using default_scanner_t = scanner<default_matcher_t>;

}  // namespace sk::scanner


#endif
