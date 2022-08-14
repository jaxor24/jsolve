#include <stdexcept>
#include <string>

namespace jsolve
{
class SolveError : public std::runtime_error
{
  public:
    explicit SolveError(const std::string& message);
    virtual ~SolveError();
};
} // namespace jsolve