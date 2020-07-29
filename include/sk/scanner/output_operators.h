#ifndef SK_LIB_SCANNER_OUTPUT_OPERATORS_H
#define SK_LIB_SCANNER_OUTPUT_OPERATORS_H

#include <ostream>

#include "sk/scanner/types.h"


std::ostream &operator<<(std::ostream &out, sk::scanner::types::ScanToken token);
std::ostream &operator<<(std::ostream &out, sk::scanner::types::StepResult token);


#endif
