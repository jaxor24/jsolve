#include "model.h"

#include <string>
#include <filesystem>
#include <stdexcept>

namespace jsolve
{
	class MPSError : public std::runtime_error
	{
	public:
		explicit MPSError(const std::string& message);
		virtual ~MPSError() throw ();
	};

	jsolve::Model read_mps(std::filesystem::path path);
}
