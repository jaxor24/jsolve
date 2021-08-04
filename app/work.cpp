#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"
#include "mps.h"

#include "matrix.h"

void go(std::filesystem::path file)
{
	auto model = jsolve::read_mps(file);

	// auto model = models::make_model_7();
	log()->debug(model.to_long_string());

	auto solution = jsolve::simplex::primal_solve(model);

	if (solution)
	{
		jsolve::log_solution(info_logger(), solution.value());
	}

}
