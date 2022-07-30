#include <string>
#include <stdexcept>

class MatrixError : public std::runtime_error
{
public:
	explicit MatrixError(const std::string& message) 
		:
		std::runtime_error(message)
	{}

	virtual ~MatrixError() throw ()
	{}
};
