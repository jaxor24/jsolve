#include <stdexcept>

#include "logging.h"

class MatrixError : public std::runtime_error
{
public:
	template <typename... Args>
	explicit MatrixError(Args&&... args)
		:
		std::runtime_error(fmt::format(std::forward<Args>(args)...))
	{}
	virtual ~MatrixError() throw () {}
};
