#include "logging.h"
#include "CommandLine.hpp"
#include "work.h"

int main(int argc, char* argv[])
{
	std::string mps_path;
	std::string log_level;

	{
		CommandLine args("jsolve");
		args.addArgument({ "-l", "--log" }, &log_level, "Log level [off, info, debug]");
		args.addArgument({ "-m", "--mps" }, &mps_path, "Path to MPS file.");

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

	logging::init_logging(log_level);
	log()->info("Starting jsolve...");

	go(mps_path);

	log()->info("Done.");
	return 0;
}
