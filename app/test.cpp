#include "test.h"

#include "logging.h"

#include "jsolve.h"

#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	auto v = std::make_shared<jsolve::Variable>(jsolve::Variable::Type::LINEAR);
	auto c = jsolve::Constraint(jsolve::Constraint::Type::LESS);

	c.add_to_lhs(1.0, v);
	c.add_to_lhs(2.0, v);
	c.add_to_lhs(3.0, v);

	Matr lhs{ 1, 3, 3.0 };
	Matr rhs{ 3, 1, 4.0 };

	log()->info("cons: {}", c);

}