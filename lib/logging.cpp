#include "logging.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

#include <stdexcept>

namespace logging
{
void init_logging(std::string level)
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
    file_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

    auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

    logger->set_pattern("[%n][%l] %v");

    if (level == "off")
    {
        logger->set_level(spdlog::level::off);
    }
    else if (level == "debug")
    {
        logger->set_level(spdlog::level::debug);
    }
    else if (level == "info")
    {
        logger->set_level(spdlog::level::info);
    }
    else if (level == "trace")
    {
        logger->set_level(spdlog::level::trace);
    }
    else
    {
        logger->set_level(spdlog::level::info);
    }

    spdlog::register_logger(logger);
}

void teardown_logging()
{
    spdlog::shutdown();
}
} // namespace logging