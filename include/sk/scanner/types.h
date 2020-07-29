#ifndef SK_LIB_SCANNER_TYPES_H
#define SK_LIB_SCANNER_TYPES_H

#include <string>
#include <variant>
#include <vector>


namespace sk::scanner::types {

    enum class ScanToken : int {
        NONE,       //
        NUMBER,     //
        FLOAT,      //
        WHITESPACE  //
    };

    enum class StepResult {
        ERROR,     //
        NO_MATCH,  //
        MATCH      //
    };


    struct Symbol {
        ScanToken token;
        std::string str {};
    };


    using GoodScanResult = std::vector<Symbol>;
    using BadScanResult  = std::monostate;
    using ScanResult     = std::variant<GoodScanResult, BadScanResult>;

}  // namespace sk::scanner::types


#endif
