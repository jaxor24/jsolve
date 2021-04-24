#include "test.h"

#include "logging.h"
#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	Matr lhs{ 1, 3, 3.0 };
	Matr rhs{ 3, 1, 4.0 };

	log()->info("lhs: {}", lhs);
	log()->info("rhs: {}", rhs);
	log()->info("product a: {}", lhs * rhs);
	log()->info("product b: {}", rhs * lhs);
}