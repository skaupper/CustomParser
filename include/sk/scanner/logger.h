#ifndef SK_LIB_SCANNER_LOGGER_H
#define SK_LIB_SCANNER_LOGGER_H

#include <spdlog/spdlog.h>


namespace sk::scanner {

    constexpr auto LOGGER_NAME {"sklib-scanner"};

    void set_log_level(spdlog::level::level_enum);
    std::shared_ptr<spdlog::logger> get_logger();

}  // namespace sk::scanner


#endif
