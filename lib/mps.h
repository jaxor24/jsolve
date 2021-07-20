#include "model.h"

#include <filesystem>
#include <stdexcept>

namespace jsolve
{
	class MPSError : public std::runtime_error
	{
	public:
		template <typename... Args>
		explicit MPSError(Args&&... args);
		virtual ~MPSError() throw ();
	};

	jsolve::Model read_mps(std::filesystem::path path);
}
