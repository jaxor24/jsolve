#pragma once

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace logging
{
void init_logging(std::string level);
void teardown_logging();
} // namespace logging

inline auto log()
{
    return spdlog::get("logger");
}

inline auto info_logger()
{
    return []<typename... Args>(fmt::format_string<Args...> format, Args&&... args) {
        log()->info(format, std::forward<decltype(args)>(args)...);
    };
}

inline auto debug_logger()
{
    return []<typename... Args>(fmt::format_string<Args...> format, Args&&... args) {
        log()->debug(format, std::forward<decltype(args)>(args)...);
    };
}