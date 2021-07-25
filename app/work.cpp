#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"
#include "mps.h"

#include "matrix.h"

void go()
{
	auto model = models::make_model_7();
	log()->debug(model);

	auto solution = jsolve::simplex::primal_solve(model);

	if (solution)
	{
		jsolve::log_solution(info_logger(), solution.value());
	}

}

void solve_mps_file(std::filesystem::path file)
{
	auto model = jsolve::read_mps(file);

	log()->debug(model);

	auto solution = jsolve::simplex::primal_solve(model);

	if (solution)
	{
		jsolve::log_solution(info_logger(), solution.value());
	}
}