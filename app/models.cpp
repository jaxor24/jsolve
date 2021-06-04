#include "models.h"


namespace models
{
	jsolve::Model make_model_1()
	{
		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Example");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemA");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemB");
		auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "ItemC");

		// Objective
		v1->cost() = 5;
		v2->cost() = 4;
		v3->cost() = 3;

		// Constraint 1
		auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
		c1->rhs() = 5;
		c1->add_to_lhs(2, v1);
		c1->add_to_lhs(3, v2);
		c1->add_to_lhs(1, v3);

		// Constraint 2
		auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
		c2->rhs() = 11;
		c2->add_to_lhs(4, v1);
		c2->add_to_lhs(1, v2);
		c2->add_to_lhs(2, v3);

		// Constraint 3
		auto* c3 = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
		c3->rhs() = 8;
		c3->add_to_lhs(3, v1);
		c3->add_to_lhs(4, v2);
		c3->add_to_lhs(2, v3);

		return m;
	}

}