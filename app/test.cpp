#include "test.h"

#include "logging.h"

#include "jsolve.h"

#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	auto v = jsolve::Variable(jsolve::Variable::Type::LINEAR, 0.0);
	auto c = jsolve::Constraint(jsolve::Constraint::Type::LESS);

	v.cost() = 6;
	c.rhs() = 6;

	Matr lhs{ 1, 3, 3.0 };
	Matr rhs{ 3, 1, 4.0 };

	log()->info("lhs: {}", lhs);
	log()->info("rhs: {}", rhs);
	log()->info("product a: {}", lhs * rhs);
	log()->info("product b: {}", rhs * lhs);
}