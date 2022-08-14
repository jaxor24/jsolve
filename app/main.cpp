﻿#include "CommandLine.hpp"
#include "logging.h"
#include "work.h"

#include "tools.h"

int main(int argc, char* argv[])
{
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
