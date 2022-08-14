#include "solve_error.h"


namespace jsolve
{
	SolveError::SolveError(const std::string& message)
		:
		std::runtime_error(message)
	{}

	SolveError::~SolveError() = default;
}