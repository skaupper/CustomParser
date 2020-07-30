#ifndef SK_LIB_SCANNER_TYPES_H
#define SK_LIB_SCANNER_TYPES_H

#include <string>
#include <variant>
#include <vector>


namespace sk::scanner::types {

    enum class StepResult {
        ERROR,     //
        NO_MATCH,  //
        MATCH      //
    };


    using TokenId = size_t;
    constexpr TokenId INVALID_TOKEN {0};

    struct Symbol {
        TokenId tokenId;
        std::string tokenStr;
        std::string str {};
    };

    struct ScanError {
        size_t line;
        size_t col;
        std::string str;
    };


    using GoodScanResult = std::vector<Symbol>;
    using BadScanResult  = ScanError;
    using ScanResult     = std::variant<GoodScanResult, BadScanResult>;

}  // namespace sk::scanner::types


#endif
