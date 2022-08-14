#include "model.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace jsolve
{
class MPSError : public std::runtime_error
{
  public:
    explicit MPSError(const std::string& message);
    virtual ~MPSError();
};

jsolve::Model read_mps(std::filesystem::path path);

} // namespace jsolve
