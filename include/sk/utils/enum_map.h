#ifndef SK_LIB_UTILS_ENUM_MAP_H
#define SK_LIB_UTILS_ENUM_MAP_H

#include <map>


namespace sk::utils {

    namespace detail {

        struct EnumClassComparer final {
            template<typename T>
            bool operator()(T e1, T e2) const {
                return static_cast<size_t>(e1) < static_cast<size_t>(e2);
            }
        };

    }  // namespace detail


    template<typename T>
    using enum_map = std::map<T, const char *const, detail::EnumClassComparer>;

}  // namespace sk::utils


#endif
