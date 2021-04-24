#include "logging.h"


int main()
{
	logging::init_loggers();
	logger()->info("Starting...");
	return 0;
}
