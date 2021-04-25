#include "work.h"

#include "logging.h"

#include "jsolve.h"

#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	auto m = jsolve::Model(jsolve::Model::Sense::MIN, "Knapsack");

	auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemA");
	auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemB");
	auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemC");

	// Objective
	v1->cost() = 10;
	v2->cost() = 20;
	v3->cost() = 5;

	// Capacity
	auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "Capacity");
	c1->rhs() = 50;
	c1->add_to_lhs(15.0, v1);
	c1->add_to_lhs(25.0, v2);
	c1->add_to_lhs(10.0, v3);

	log()->info(m);

}