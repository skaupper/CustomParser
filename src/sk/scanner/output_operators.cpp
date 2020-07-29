#include "sk/scanner/output_operators.h"

#include "sk/utils/enum_map.h"

#include <spdlog/spdlog.h>


std::ostream &operator<<(std::ostream &out, sk::scanner::types::ScanToken token) {
    using namespace sk::utils;
    using namespace sk::scanner::types;

    static const enum_map<ScanToken> enumToStringMap {
        {ScanToken::NUMBER, "NUMBER"},         //
        {ScanToken::FLOAT, "FLOAT"},           //
        {ScanToken::WHITESPACE, "WHITESPACE"}  //
    };

    const auto it {enumToStringMap.find(token)};
    if (it == std::cend(enumToStringMap)) {
        spdlog::debug("operator<<(std::ostream &, ScanToken): Could not find given token!");
        out << "<UNKNOWN_TOKEN>";
        return out;
    }

    out << it->second;
    return out;
}

std::ostream &operator<<(std::ostream &out, sk::scanner::types::StepResult token) {
    using namespace sk::utils;
    using namespace sk::scanner::types;

    static const enum_map<StepResult> enumToStringMap {
        {StepResult::ERROR, "ERROR"},        //
        {StepResult::NO_MATCH, "NO_MATCH"},  //
        {StepResult::MATCH, "MATCH"}         //
    };

    const auto it {enumToStringMap.find(token)};
    if (it == std::cend(enumToStringMap)) {
        spdlog::debug("operator<<(std::ostream &, StepResult): Could not find given enum variant!");
        out << "<UNKNOWN>";
        return out;
    }

    out << it->second;
    return out;
}
