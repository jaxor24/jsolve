#include <string>
#include <stdexcept>

namespace jsolve
{
	class SolveError : public std::runtime_error
	{
	public:
		explicit SolveError(const std::string& message);
		virtual ~SolveError() throw ();
	};
}