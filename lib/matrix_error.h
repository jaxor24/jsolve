#include <stdexcept>
#include <string>

class MatrixError : public std::runtime_error
{
  public:
    explicit MatrixError(const std::string& message);
    virtual ~MatrixError();
};
