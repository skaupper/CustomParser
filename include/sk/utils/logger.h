#ifndef SK_LIB_UTILS_LOGGER_H
#define SK_LIB_UTILS_LOGGER_H

#include <spdlog/spdlog.h>


namespace sk::utils {

    constexpr auto LOGGER_NAME {"sklib-utils"};

    void set_log_level(spdlog::level::level_enum);
    std::shared_ptr<spdlog::logger> get_logger();

}  // namespace sk::utils


#endif
