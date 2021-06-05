#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"

void go()
{
	auto model = models::make_model_1();
	log()->info(model);

	auto solution = jsolve::simplex::primal_solve(model);
	if (solution) { jsolve::log_solution(get_logger(), solution.value()); }

}