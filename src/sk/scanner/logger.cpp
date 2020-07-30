#include "sk/scanner/logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>


static auto logger_scanner {spdlog::stdout_color_mt(sk::scanner::LOGGER_NAME)};


namespace sk::scanner {

    std::shared_ptr<spdlog::logger> get_logger() { return spdlog::get(LOGGER_NAME); }

    void set_log_level(spdlog::level::level_enum lvl) { get_logger()->set_level(lvl); }

}  // namespace sk::scanner
