#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace logging
{
	void init_logging();
}

inline auto log()
{
    return spdlog::get("logger");
}
