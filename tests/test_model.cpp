#include "test_includes.h"

#include "model.h"


TEST_CASE("Model::Model()", "[variable]")
{
    SECTION("valid construction")
    {
        auto m = jsolve::Model{ jsolve::Model::Sense::MIN, "Example" };

        REQUIRE(m.name() == "Example");
        REQUIRE(m.sense() == jsolve::Model::Sense::MIN);
        REQUIRE(m.objective_name().empty());
        REQUIRE(m.get_variables().empty());
        REQUIRE(m.get_constraints().empty());
    }

    SECTION("Model::make_variable")
    {
        auto m = jsolve::Model{ jsolve::Model::Sense::MIN, "Example" };

        SECTION("one variable")
        {
            auto* v1 = m.make_variable(jsolve::Variable::Type::LINEAR, "x1");
            REQUIRE(m.get_variables().size() == 1);
            REQUIRE(m.get_variables().front().get() == v1);
        }

        SECTION("two variables")
        {
            SECTION("alphbetical sorting")
            {
                auto* b = m.make_variable(jsolve::Variable::Type::LINEAR, "b");
                auto* a = m.make_variable(jsolve::Variable::Type::LINEAR, "a");
                REQUIRE(m.get_variables().size() == 2);
                REQUIRE(m.get_variables().at(0).get() == a);
                REQUIRE(m.get_variables().at(1).get() == b);
            }

            SECTION("invalid - same name")
            {
                m.make_variable(jsolve::Variable::Type::LINEAR, "a");
                REQUIRE_THROWS_AS(m.make_variable(jsolve::Variable::Type::LINEAR, "a"), jsolve::ModelError);
            }
        }
    }

    SECTION("Model::make_constraint")
    {
        auto m = jsolve::Model{ jsolve::Model::Sense::MIN, "Example" };

        SECTION("one constraint")
        {
            auto* c1 = m.make_constraint(jsolve::Constraint::Type::LESS, "a");
            REQUIRE(m.get_constraints().size() == 1);
            REQUIRE(m.get_constraints().front().get() == c1);
        }

        SECTION("two constraints")
        {
            SECTION("alphbetical sorting")
            {
                auto* b = m.make_constraint(jsolve::Constraint::Type::LESS, "b");
                auto* a = m.make_constraint(jsolve::Constraint::Type::LESS, "a");
                REQUIRE(m.get_constraints().size() == 2);
                REQUIRE(m.get_constraints().at(0).get() == a);
                REQUIRE(m.get_constraints().at(1).get() == b);
            }

            SECTION("invalid - same name")
            {
                m.make_constraint(jsolve::Constraint::Type::LESS, "a");
                REQUIRE_THROWS_AS(m.make_constraint(jsolve::Constraint::Type::LESS, "a"), jsolve::ModelError);
            }
        }
    }

    SECTION("Model::objective_name")
    {
        auto m = jsolve::Model{ jsolve::Model::Sense::MIN, "Example" };

        SECTION("basic")
        {
            REQUIRE(m.objective_name().empty());
            m.objective_name() = "test";
            REQUIRE(m.objective_name() == "test");
        }
    }
}