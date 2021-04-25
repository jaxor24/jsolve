#include "test.h"

#include "logging.h"

#include "jsolve.h"

#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	auto m = jsolve::Model(jsolve::Model::Sense::MIN);

	auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "v1");
	auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "v2");

	auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "capacity");

	c1->add_to_lhs(1.0, v1);
	c1->add_to_lhs(2.0, v2);


	log()->info("cons: {}", *c1);

}