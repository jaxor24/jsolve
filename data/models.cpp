#include "models.h"


namespace models
{
	jsolve::Model make_model_1()
	{
		// Model from:
		// p11. Linear Programming (2014) Vanderbei.
		// Model is solveable in 2 interations.

		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Example");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
		auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

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

	jsolve::Model make_model_2()
	{
		// Model from:
		// p17. Linear Programming (2014) Vanderbei.
		// Model has initial infeasible dictionary.

		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Example");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

		// Objective
		v1->cost() = -2;
		v2->cost() = -1;

		// Constraint 1
		auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
		c1->rhs() = -1;
		c1->add_to_lhs(-1, v1);
		c1->add_to_lhs(1, v2);

		// Constraint 2
		auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
		c2->rhs() = -2;
		c2->add_to_lhs(-1, v1);
		c2->add_to_lhs(-2, v2);

		// Constraint 3
		auto* c3 = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
		c3->rhs() = 1;
		c3->add_to_lhs(1, v2);

		return m;
	}

	jsolve::Model make_model_3()
	{
		// Model from:
		// Problem 2.1 from Linear Programming (2014) Vanderbei.

		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "P2.1 LP 2014");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
		auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
		auto* v4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");

		// Objective
		v1->cost() = 6;
		v2->cost() = 8;
		v3->cost() = 5;
		v4->cost() = 9;

		// Constraint 1
		auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
		c1->rhs() = 5;
		c1->add_to_lhs(2, v1);
		c1->add_to_lhs(1, v2);
		c1->add_to_lhs(1, v3);
		c1->add_to_lhs(3, v4);

		// Constraint 2
		auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
		c2->rhs() = 3;
		c2->add_to_lhs(1, v1);
		c2->add_to_lhs(3, v2);
		c2->add_to_lhs(1, v3);
		c2->add_to_lhs(2, v4);

		return m;
	}

	jsolve::Model make_model_4()
	{
		// Model from:
		// Problem 2.2 from Linear Programming (2014) Vanderbei.

		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "P2.2 LP 2014");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

		v1->cost() = 2;
		v2->cost() = 1;

		auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
		c1->rhs() = 4;
		c1->add_to_lhs(2, v1);
		c1->add_to_lhs(1, v2);

		auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
		c2->rhs() = 3;
		c2->add_to_lhs(2, v1);
		c2->add_to_lhs(3, v2);

		auto* c3 = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
		c3->rhs() = 5;
		c3->add_to_lhs(4, v1);
		c3->add_to_lhs(1, v2);

		auto* c4 = m.make_constraint(jsolve::Constraint::Type::LESS, "C4");
		c4->rhs() = 1;
		c4->add_to_lhs(1, v1);
		c4->add_to_lhs(5, v2);

		return m;
	}

	jsolve::Model make_model_5()
	{
		// Model from:
		// Problem 2.3 from Linear Programming (2014) Vanderbei.

		auto m = jsolve::Model(jsolve::Model::Sense::MAX, "P2.3 LP 2014");

		auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
		auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
		auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

		v1->cost() = 2;
		v2->cost() = -6;

		auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
		c1->rhs() = -2;
		c1->add_to_lhs(-1, v1);
		c1->add_to_lhs(-1, v2);
		c1->add_to_lhs(-1, v3);

		auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
		c2->rhs() = 3;
		c2->add_to_lhs(2, v1);
		c2->add_to_lhs(-1, v2);
		c2->add_to_lhs(1, v3);

		return m;
	}
}