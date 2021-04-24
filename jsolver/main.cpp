#include "logging.h"


int main()
{
	logging::init_logging();
	log()->info("Starting...");
	return 0;
}
