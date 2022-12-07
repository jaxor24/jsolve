#include <optional>
#include <utility>

class Range
{
  public:
    Range() = default;
    Range(std::size_t value);
    Range(std::size_t start, std::size_t end);

    std::size_t start() const;
    std::size_t end() const;
    std::size_t size() const;

    operator bool() const;

  private:
    std::optional<std::pair<std::size_t, std::size_t>> m_data;
};
