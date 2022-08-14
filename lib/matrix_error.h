#include <string>
#include <stdexcept>

class MatrixError : public std::runtime_error
{
public:
	explicit MatrixError(const std::string& message);
	virtual ~MatrixError();
};
