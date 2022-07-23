#include "model.h"

#include "logging.h"

#include <filesystem>
#include <stdexcept>

namespace jsolve
{
	class MPSError : public std::runtime_error
	{
	public:
		template <typename... Args>
		explicit MPSError(fmt::format_string<Args...> format, Args&&... args)
			:
			std::runtime_error(fmt::format(format, std::forward<Args>(args)...))
		{}

		virtual ~MPSError() throw ()
		{}
	};

	jsolve::Model read_mps(std::filesystem::path path);
}
