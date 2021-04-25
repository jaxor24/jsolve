#include "logging.h"

#include "work.h"

int main()
{
	logging::init_logging();
	log()->info("Starting...");
	do_work();
	log()->info("Done.");
	return 0;
}
