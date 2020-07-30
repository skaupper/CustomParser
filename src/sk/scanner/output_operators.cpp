#include "sk/scanner/output_operators.h"
#include "sk/scanner/logger.h"

#include "sk/utils/enum_map.h"


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
        sk::scanner::get_logger()->debug("operator<<(std::ostream &, StepResult): Could not find given enum variant!");
        out << "<UNKNOWN>";
        return out;
    }

    out << it->second;
    return out;
}
