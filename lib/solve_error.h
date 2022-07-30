#include <string>
#include <stdexcept>

namespace jsolve
{
	class SolveError : public std::runtime_error
	{
	public:
		explicit SolveError(const std::string& message)
			:
			std::runtime_error(message)
		{}

		virtual ~SolveError() throw ()
		{}
	};
}