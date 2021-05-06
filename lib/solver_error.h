#include "logging.h"

#include <stdexcept>

namespace jsolve
{
	class SolveError : public std::runtime_error
	{
	public:
		template <typename... Args>
		explicit SolveError(Args&&... args)
			:
			std::runtime_error(fmt::format(std::forward<Args>(args)...))
		{}
		virtual ~SolveError() throw () {}
	};
}