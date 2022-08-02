#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

namespace logging
{
	void init_logging(std::string level);
	void teardown_logging();
}

inline auto log()
{
    return spdlog::get("logger");
}

inline auto info_logger()
{
	return []<typename... Args>(fmt::format_string<Args...> format, Args&&... args) { log()->info(format, std::forward<decltype(args)>(args)...); };
}

inline auto debug_logger()
{
	return []<typename... Args>(fmt::format_string<Args...> format, Args&&... args) { log()->debug(format, std::forward<decltype(args)>(args)...); };
}