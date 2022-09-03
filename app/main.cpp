#include "CommandLine.hpp"
#include "logging.h"
#include "work.h"

#include "tools.h"

#include <experimental/mdspan>
#include <iostream>

namespace stdex = std::experimental;

void foo()
{
    std::array d{
        0, 5, 1, 3, 8, 4, 2, 7, 6,
    };

    stdex::mdspan m{d.data(), stdex::extents{3, 3}};

    for (std::size_t i = 0; i < m.extent(0); ++i)
    {
        for (std::size_t j = 0; j < m.extent(1); ++j)
        {
            std::cout << "m(" << i << ", " << j << ") == " << m(i, j) << "\n";
        }
    }
}

int main(int argc, char* argv[])
{
    // foo();

    std::string mps_path;
    std::string log_level;

    {
        CommandLine args("jsolve");
        args.addArgument({"-l", "--log"}, &log_level, "Log level [off, info, debug]");
        args.addArgument({"-m", "--mps"}, &mps_path, "Path to MPS file.");

        try
        {
            args.parse(argc, argv);
        }
        catch (std::runtime_error const& e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }
    }

    try
    {
        logging::init_logging(log_level);
        Timer timer{info_logger(), "Running jsolve"};
        go(mps_path);
    }
    catch (std::exception const& e)
    {
        log()->error("Exception: {}", e.what());
        return -1;
    }

    return 0;
}
