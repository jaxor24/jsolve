#include <filesystem>
#include <source_location>

inline std::filesystem::path get_mps(std::string file_name)
{
    return std::filesystem::path{std::source_location::current().file_name()}.remove_filename() / "mps_examples" /
           file_name;
}