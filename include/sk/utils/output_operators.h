#ifndef SK_LIB_UTILS_OUTPUT_OPERATORS_H
#define SK_LIB_UTILS_OUTPUT_OPERATORS_H

#include <optional>
#include <ostream>


//
// std::optional
//

template<typename T>
inline std::ostream &operator<<(std::ostream &out, const std::optional<T> &opt) {
    if (!opt) {
        out << "<empty>";
    } else {
        out << opt.value();
    }
    return out;
}

inline std::ostream &operator<<(std::ostream &out, std::nullopt_t) {
    out << "<nullopt>";
    return out;
}


#endif
