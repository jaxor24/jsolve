#include "logging.h"

#include "work.h"

int main()
{
	logging::init_logging();
	log()->info("Starting jsolve...");
	go();
	log()->info("Done.");
	return 0;
}
