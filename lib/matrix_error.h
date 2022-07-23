#include "logging.h"

#include <stdexcept>

class MatrixError : public std::runtime_error
{
public:
	template <typename... Args>
	explicit MatrixError(fmt::format_string<Args...> format, Args&&... args) 
		:
		std::runtime_error(fmt::format(format, std::forward<Args>(args)...))
	{}

	virtual ~MatrixError() throw ()
	{}
};
