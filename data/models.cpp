#include "models.h"

namespace models
{
jsolve::Model make_model_0()
{
    // Model from:
    // p86. Linear Programming (2014) Vanderbei.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Example");

    auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

    // Objective
    v1->cost() = 4;
    v2->cost() = 3;

    // Constraint 1
    auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
    c1->rhs() = 1;
    c1->add_to_lhs(1, v1);
    c1->add_to_lhs(-1, v2);

    // Constraint 2
    auto* c2 = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
    c2->rhs() = 3;
    c2->add_to_lhs(2, v1);
    c2->add_to_lhs(-1, v2);

    // Constraint 3
    auto* c3 = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
    c3->rhs() = 5;
    c3->add_to_lhs(1, v2);

    return m;
}

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
    // Model has initial infeasible dictionary, requiring a 2 phase algorithm.

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
    c2->rhs() = 1;
    c2->add_to_lhs(2, v1);
    c2->add_to_lhs(-1, v2);
    c2->add_to_lhs(1, v3);

    return m;
}

jsolve::Model make_model_6()
{
    // Model from:
    // Problem 2.8 from Linear Programming (2014) Vanderbei.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "P2.8 LP 2014");

    auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

    v1->cost() = 3;
    v2->cost() = 2;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 1;
        c->add_to_lhs(+1, v1);
        c->add_to_lhs(-2, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 2;
        c->add_to_lhs(+1, v1);
        c->add_to_lhs(-1, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 6;
        c->add_to_lhs(+2, v1);
        c->add_to_lhs(-1, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C4");
        c->rhs() = 5;
        c->add_to_lhs(+1, v1);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C5");
        c->rhs() = 16;
        c->add_to_lhs(+2, v1);
        c->add_to_lhs(+1, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C6");
        c->rhs() = 12;
        c->add_to_lhs(+1, v1);
        c->add_to_lhs(+1, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C7");
        c->rhs() = 21;
        c->add_to_lhs(+1, v1);
        c->add_to_lhs(+2, v2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C8");
        c->rhs() = 10;
        c->add_to_lhs(+1, v2);
    }

    return m;
}

jsolve::Model make_model_7()
{
    // Model from:
    // Problem 2.10 from Linear Programming (2014) Vanderbei.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "P2.10 LP 2014");

    auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* v2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* v3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* v4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");

    v1->cost() = 6;
    v2->cost() = 8;
    v3->cost() = 5;
    v4->cost() = 9;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 1;
        c->add_to_lhs(1.0, v1);
        c->add_to_lhs(1.0, v2);
        c->add_to_lhs(1.0, v3);
        c->add_to_lhs(1.0, v4);
    }

    return m;
}

jsolve::Model make_model_8()
{
    // Model from:
    // Problem 2.11 from Linear Programming (2014) Vanderbei.

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "P2.11 LP 2014");

    auto* x12 = m.make_variable(jsolve::Variable::Type::LINEAR, "x12");
    auto* x13 = m.make_variable(jsolve::Variable::Type::LINEAR, "x13");
    auto* x14 = m.make_variable(jsolve::Variable::Type::LINEAR, "x14");
    auto* x23 = m.make_variable(jsolve::Variable::Type::LINEAR, "x23");
    auto* x24 = m.make_variable(jsolve::Variable::Type::LINEAR, "x24");
    auto* x34 = m.make_variable(jsolve::Variable::Type::LINEAR, "x34");

    x12->cost() = 1;
    x13->cost() = 8;
    x14->cost() = 9;
    x23->cost() = 2;
    x24->cost() = 7;
    x34->cost() = 3;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::GREAT, "C1");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x12);
        c->add_to_lhs(1.0, x13);
        c->add_to_lhs(1.0, x14);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 0;
        c->add_to_lhs(-1.0, x12);
        c->add_to_lhs(1.0, x23);
        c->add_to_lhs(1.0, x24);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 0;
        c->add_to_lhs(-1.0, x13);
        c->add_to_lhs(-1.0, x23);
        c->add_to_lhs(1.0, x34);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C4");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x14);
        c->add_to_lhs(1.0, x24);
        c->add_to_lhs(1.0, x34);
    }

    return m;
}

jsolve::Model make_model_9()
{
    // Model from 'Vincent Conitzer' pg.3 simplex lecture notes.
    // Contains a degenerate pivot.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Degenerate Example");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* x4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");
    auto* x5 = m.make_variable(jsolve::Variable::Type::LINEAR, "x5");

    x1->cost() = 4;
    x2->cost() = 5;
    x3->cost() = 4;
    x4->cost() = 7;
    x5->cost() = 1;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x1);
        c->add_to_lhs(1.0, x3);
        c->add_to_lhs(1.0, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x1);
        c->add_to_lhs(1.0, x2);
        c->add_to_lhs(1.0, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x2);
        c->add_to_lhs(1.0, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C4");
        c->rhs() = 1;
        c->add_to_lhs(1.0, x4);
        c->add_to_lhs(1.0, x5);
    }

    return m;
}

jsolve::Model make_model_10()
{
    // Model from 'Vincent Conitzer' pg.7 simplex lecture notes.
    // Requires a phase 1.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "NEEDS PHASE 1");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

    x1->cost() = 3;
    x2->cost() = 2;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 16;
        c->add_to_lhs(4, x1);
        c->add_to_lhs(2, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 8;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(2, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 5;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C4");
        c->rhs() = -2;
        c->add_to_lhs(-1, x1);
        c->add_to_lhs(-1, x2);
    }

    return m;
}

jsolve::Model make_model_11()
{
    // Model from Winston, OR. pg. 172.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "COULD CYCLE");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* x4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");

    x1->cost() = -3;
    x2->cost() = 1;
    x3->cost() = -6;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 0;
        c->add_to_lhs(9, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(-9, x3);
        c->add_to_lhs(-2, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 0;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1.0 / 3, x2);
        c->add_to_lhs(-2, x3);
        c->add_to_lhs(-1.0 / 3, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 1;
        c->add_to_lhs(-9, x1);
        c->add_to_lhs(-1, x2);
        c->add_to_lhs(9, x3);
        c->add_to_lhs(2, x4);
    }

    return m;
}

jsolve::Model make_model_12()
{
    // Model from Winston, OR. pg. 179.

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "TWO PHASE BEVCO");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

    x1->cost() = 2;
    x2->cost() = 3;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 4;
        c->add_to_lhs(1.0 / 2, x1);
        c->add_to_lhs(1.0 / 4, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::GREAT, "C2");
        c->rhs() = 20;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(3, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 10;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
    }

    return m;
}

jsolve::Model make_model_13()
{
    // Model from Winston, OR. pg. 183.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "TWO PHASE SIMPLEX");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* x4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");
    auto* x5 = m.make_variable(jsolve::Variable::Type::LINEAR, "x5");
    auto* x6 = m.make_variable(jsolve::Variable::Type::LINEAR, "x6");

    x1->cost() = 40;
    x2->cost() = 10;
    x3->cost() = 0;
    x4->cost() = 0;
    x5->cost() = 7;
    x6->cost() = 14;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 0;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(-1, x2);
        c->add_to_lhs(2, x5);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 0;
        c->add_to_lhs(-2, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(-2, x5);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 3;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x3);
        c->add_to_lhs(1, x5);
        c->add_to_lhs(-1, x6);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C4");
        c->rhs() = 4;
        c->add_to_lhs(2, x2);
        c->add_to_lhs(1, x3);
        c->add_to_lhs(1, x4);
        c->add_to_lhs(2, x5);
        c->add_to_lhs(1, x6);
    }

    return m;
}

jsolve::Model make_model_14()
{
    // Model from Winston, OR. pg. 159. Unbounded.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "UNBONDED");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");

    x2->cost() = 2;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 4;
        c->add_to_lhs(+1, x1);
        c->add_to_lhs(-1, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 1;
        c->add_to_lhs(-1, x1);
        c->add_to_lhs(+1, x2);
    }

    return m;
}

jsolve::Model make_model_15()
{
    // Model from Luenberger & Ye, 4th ed, pg. 47.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "SIMPLE");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 3;
    x2->cost() = 1;
    x3->cost() = 3;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 2;
        c->add_to_lhs(+2, x1);
        c->add_to_lhs(+1, x2);
        c->add_to_lhs(+1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 5;
        c->add_to_lhs(+1, x1);
        c->add_to_lhs(+2, x2);
        c->add_to_lhs(+3, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 5;
        c->add_to_lhs(+2, x1);
        c->add_to_lhs(+2, x2);
        c->add_to_lhs(+1, x3);
    }

    return m;
}

jsolve::Model make_model_16()
{
    // Model from Luenberger & Ye, 4th ed, pg. 51.

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "SIMPLE");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 4;
    x2->cost() = 1;
    x3->cost() = 1;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 4;
        c->add_to_lhs(+2, x1);
        c->add_to_lhs(+1, x2);
        c->add_to_lhs(+2, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 3;
        c->add_to_lhs(+3, x1);
        c->add_to_lhs(+3, x2);
        c->add_to_lhs(+1, x3);
    }

    return m;
}

jsolve::Model make_model_17()
{
    // Model from unnamed simplex notes "Linear Programming Notes III".

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* x4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");

    x1->cost() = 2;
    x2->cost() = 4;
    x3->cost() = 3;
    x4->cost() = 1;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 12;
        c->add_to_lhs(3, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(-1, x3);
        c->add_to_lhs(4, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 7;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(-3, x2);
        c->add_to_lhs(2, x3);
        c->add_to_lhs(3, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 10;
        c->add_to_lhs(2, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(3, x3);
        c->add_to_lhs(-1, x4);
    }

    return m;
}

jsolve::Model make_model_18()
{
    // Model from pg 77 of "Applied Mathematical Programming" Chapter 2.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 5;
    x2->cost() = 4.5;
    x3->cost() = 6;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 60;
        c->add_to_lhs(6, x1);
        c->add_to_lhs(5, x2);
        c->add_to_lhs(8, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = 150;
        c->add_to_lhs(10, x1);
        c->add_to_lhs(20, x2);
        c->add_to_lhs(10, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C3");
        c->rhs() = 8;
        c->add_to_lhs(1, x1);
    }

    return m;
}

jsolve::Model make_model_19()
{
    // Model from pg 62 of "Applied Mathematical Programming" Chapter 2.
    // Demonstrates edge cases in Phase 1->2 transition

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");
    auto* x4 = m.make_variable(jsolve::Variable::Type::LINEAR, "x4");

    x1->cost() = -3;
    x3->cost() = 1;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 4;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(1, x3);
        c->add_to_lhs(1, x4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 1;
        c->add_to_lhs(-2, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(-1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 9;
        c->add_to_lhs(3, x2);
        c->add_to_lhs(1, x3);
        c->add_to_lhs(1, x4);
    }
    return m;
}

jsolve::Model make_model_20()
{
    // Model from pg 72 of "Applied Mathematical Programming" Chapter 2.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 2;
    x2->cost() = 1;
    x3->cost() = 1;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 9;
        c->add_to_lhs(2, x1);
        c->add_to_lhs(3, x2);
        c->add_to_lhs(-1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::GREAT, "C2");
        c->rhs() = 4;
        c->add_to_lhs(2, x2);
        c->add_to_lhs(1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 6;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x3);
    }

    return m;
}

jsolve::Model make_model_21()
{
    // Model from pg 73 of "Applied Mathematical Programming" Chapter 2.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = -3;
        c->add_to_lhs(3, x1);
        c->add_to_lhs(2, x2);
        c->add_to_lhs(-1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C2");
        c->rhs() = -1;
        c->add_to_lhs(-1, x1);
        c->add_to_lhs(-1, x2);
        c->add_to_lhs(2, x3);
    }
    return m;
}

jsolve::Model make_model_22()
{
    // Model from pg 74 of "Applied Mathematical Programming" Chapter 2.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 2;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(-2, x2);
        c->add_to_lhs(1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 1;
        c->add_to_lhs(-1, x1);
        c->add_to_lhs(-3, x2);
        c->add_to_lhs(1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 7;
        c->add_to_lhs(2, x1);
        c->add_to_lhs(-3, x2);
        c->add_to_lhs(4, x3);
    }
    return m;
}

jsolve::Model make_model_23()
{
    // Model from pg 112 of "Applied Mathematical Programming" Chapter 3.

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "Simplex");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 28;
    x2->cost() = 30;
    x3->cost() = 20;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "C1");
        c->rhs() = 3.2;
        c->add_to_lhs(4, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(5, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::GREAT, "C2");
        c->rhs() = 3.5;
        c->add_to_lhs(3.5, x1);
        c->add_to_lhs(4, x2);
        c->add_to_lhs(3, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 1;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
        c->add_to_lhs(1, x3);
    }

    return m;
}

jsolve::Model make_model_24()
{
    // Model from pg 121 of "Applied Mathematical Programming" Chapter 3.
    // Heating Oil Allocation

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "Heating Oil");

    auto* a1 = m.make_variable(jsolve::Variable::Type::LINEAR, "a1");
    auto* a2 = m.make_variable(jsolve::Variable::Type::LINEAR, "a2");
    auto* a3 = m.make_variable(jsolve::Variable::Type::LINEAR, "a3");
    auto* a4 = m.make_variable(jsolve::Variable::Type::LINEAR, "a4");

    auto* b1 = m.make_variable(jsolve::Variable::Type::LINEAR, "b1");
    auto* b2 = m.make_variable(jsolve::Variable::Type::LINEAR, "b2");
    auto* b3 = m.make_variable(jsolve::Variable::Type::LINEAR, "b3");
    auto* b4 = m.make_variable(jsolve::Variable::Type::LINEAR, "b4");

    auto* c1 = m.make_variable(jsolve::Variable::Type::LINEAR, "c1");
    auto* c2 = m.make_variable(jsolve::Variable::Type::LINEAR, "c2");
    auto* c3 = m.make_variable(jsolve::Variable::Type::LINEAR, "c3");
    auto* c4 = m.make_variable(jsolve::Variable::Type::LINEAR, "c4");

    auto* pmax = m.make_variable(jsolve::Variable::Type::LINEAR, "pmax");

    a1->cost() = 0.1;
    a2->cost() = 0.16;
    a3->cost() = 0.32;
    a4->cost() = 0.28;

    b1->cost() = 0.2;
    b2->cost() = 0.34;
    b3->cost() = 0.3;
    b4->cost() = 0.12;

    c1->cost() = 0.34;
    c2->cost() = 0.38;
    c3->cost() = 0.22;
    c4->cost() = 0.18;

    pmax->cost() = 0;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "supply1");
        c->rhs() = 95;
        c->add_to_lhs(1, a1);
        c->add_to_lhs(1, a2);
        c->add_to_lhs(1, a3);
        c->add_to_lhs(1, a4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "supply2");
        c->rhs() = 63;
        c->add_to_lhs(1, b1);
        c->add_to_lhs(1, b2);
        c->add_to_lhs(1, b3);
        c->add_to_lhs(1, b4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "supply3");
        c->rhs() = 116;
        c->add_to_lhs(1, c1);
        c->add_to_lhs(1, c2);
        c->add_to_lhs(1, c3);
        c->add_to_lhs(1, c4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "quota1");
        c->rhs() = 55;
        c->add_to_lhs(1, a1);
        c->add_to_lhs(1, b1);
        c->add_to_lhs(1, c1);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "quota2");
        c->rhs() = 73;
        c->add_to_lhs(1, a2);
        c->add_to_lhs(1, b2);
        c->add_to_lhs(1, c2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "quota3");
        c->rhs() = 105;
        c->add_to_lhs(1, a3);
        c->add_to_lhs(1, b3);
        c->add_to_lhs(1, c3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "quota4");
        c->rhs() = 38;
        c->add_to_lhs(1, a4);
        c->add_to_lhs(1, b4);
        c->add_to_lhs(1, c4);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "aveprice1");
        c->rhs() = 0;
        c->add_to_lhs(0.1915, a1);
        c->add_to_lhs(1.701, b1);
        c->add_to_lhs(0.2260, c1);
        c->add_to_lhs(-1, pmax);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "aveprice2");
        c->rhs() = 0;
        c->add_to_lhs(0.1442, a2);
        c->add_to_lhs(0.1286, b2);
        c->add_to_lhs(0.1703, c2);
        c->add_to_lhs(-1, pmax);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "aveprice3");
        c->rhs() = 0;
        c->add_to_lhs(0.1003, a3);
        c->add_to_lhs(0.0894, b3);
        c->add_to_lhs(0.1184, c3);
        c->add_to_lhs(-1, pmax);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "aveprice4");
        c->rhs() = 0;
        c->add_to_lhs(0.2771, a4);
        c->add_to_lhs(0.2471, b4);
        c->add_to_lhs(0.3271, c4);
        c->add_to_lhs(-1, pmax);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "aveprice5");
        c->rhs() = 11.4;
        c->add_to_lhs(+1, pmax);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "shipcapacity1");
        c->rhs() = 25;
        c->add_to_lhs(+1, a2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "shipcapacity2");
        c->rhs() = 20;
        c->add_to_lhs(+1, b2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "shipcapacity3");
        c->rhs() = 20;
        c->add_to_lhs(+1, b3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::LESS, "shipcapacity4");
        c->rhs() = 25;
        c->add_to_lhs(+1, c1);
    }

    return m;
}

jsolve::Model make_model_25()
{
    // Frabicrated model with only EQ constraints
    // Min objective requires artifical vars to be driven from the basis.

    auto m = jsolve::Model(jsolve::Model::Sense::MIN, "JR_EQ_MIN");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 1;
    x2->cost() = 2;
    x3->cost() = 3;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 2;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 2;
        c->add_to_lhs(1, x2);
        c->add_to_lhs(1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 2;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x3);
    }

    return m;
}

jsolve::Model make_model_26()
{
    // Frabicrated model with only EQ constraints
    // Max objective requires two phase plus artifical vars to be driven from basis.

    auto m = jsolve::Model(jsolve::Model::Sense::MAX, "JR_EQ_MAX");

    auto* x1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
    auto* x2 = m.make_variable(jsolve::Variable::Type::LINEAR, "x2");
    auto* x3 = m.make_variable(jsolve::Variable::Type::LINEAR, "x3");

    x1->cost() = 1;
    x2->cost() = 2;
    x3->cost() = 3;

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C1");
        c->rhs() = 2;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x2);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C2");
        c->rhs() = 2;
        c->add_to_lhs(1, x2);
        c->add_to_lhs(1, x3);
    }

    {
        auto* c = m.make_constraint(jsolve::Constraint::Type::EQUAL, "C3");
        c->rhs() = 2;
        c->add_to_lhs(1, x1);
        c->add_to_lhs(1, x3);
    }

    return m;
}

} // namespace models
