#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"

#include "matrix.h"

void go()
{
	auto model = models::make_model_1();
	log()->info(model);

	Matrix<double> m1{ 3, 3, 0.0 }; 
	m1(0, 0) = 1;
	m1(0, 1) = 2;
	m1(0, 2) = 3;

	m1(1, 0) = 4;
	m1(1, 1) = 5;
	m1(1, 2) = 6;

	m1(2, 0) = 7;
	m1(2, 1) = 8;
	m1(2, 2) = 9;

	m1.foo();

	//auto solution = jsolve::simplex::primal_solve(model);

	//if (solution) 
	//{ 
	//	jsolve::log_solution(info_logger(), solution.value()); 
	//}
}