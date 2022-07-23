#include "logging.h"

#include <stdexcept>

namespace jsolve
{
	class SolveError : public std::runtime_error
	{
	public:
		template <typename... Args>
		explicit SolveError(fmt::format_string<Args...> format, Args&&... args)
			:
			std::runtime_error(fmt::format(format, std::forward<Args>(args)...))
		{}

		virtual ~SolveError() throw ()
		{}
	};
}