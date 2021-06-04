#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"

void go()
{
	auto model = models::make_model_1();
	log()->info(model);

	jsolve::simplex::primal_solve(model);

}