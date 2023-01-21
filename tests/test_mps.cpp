#include "test_includes.h"

#include "mps.h"
#include "simplex_common.h"

TEST_CASE("jsolve::mps::read_mps", "[matrix]")
{
    SECTION("model 1")
    {
        auto model{jsolve::read_mps(get_mps("example1.mps"))};

        SECTION("attributes")
        {
            REQUIRE(model.name() == "TESTPROB");
            REQUIRE(model.get_constraints().size() == 3);
            REQUIRE(model.get_variables().size() == 3);
        }

        auto* x1 = model.get_variable("XONE");
        auto* y2 = model.get_variable("YTWO");
        auto* z3 = model.get_variable("ZTHREE");

        SECTION("variables")
        {
            REQUIRE(x1->cost() == 1);
            REQUIRE(y2->cost() == 4);
            REQUIRE(z3->cost() == 9);

            // These are currently not set
            REQUIRE(x1->lower_bound() == 0);
            REQUIRE(y2->lower_bound() == 1);
            REQUIRE(z3->lower_bound() == 0);

            // These are currently not set
            REQUIRE(x1->upper_bound() == 4);
            REQUIRE(y2->upper_bound() == 1);
            REQUIRE(z3->upper_bound() == std::numeric_limits<double>::infinity());
        }

        SECTION("constraints")
        {
            {
                const auto* lim1 = model.get_constraint("LIM1");
                REQUIRE(lim1->type() == jsolve::Constraint::Type::LESS);
                REQUIRE(lim1->rhs() == 5);
                const auto& lim1_entries = lim1->entries();
                REQUIRE(lim1_entries.size() == 2);
                REQUIRE(lim1_entries.at(x1) == 1);
                REQUIRE(lim1_entries.at(y2) == 1);
            }

            {
                const auto* lim2 = model.get_constraint("LIM2");
                REQUIRE(lim2->type() == jsolve::Constraint::Type::GREAT);
                REQUIRE(lim2->rhs() == 10);
                const auto& lim2_entries = lim2->entries();
                REQUIRE(lim2_entries.size() == 2);
                REQUIRE(lim2_entries.at(x1) == 1);
                REQUIRE(lim2_entries.at(z3) == 1);
            }

            {
                const auto* myeqn = model.get_constraint("MYEQN");
                REQUIRE(myeqn->type() == jsolve::Constraint::Type::EQUAL);
                REQUIRE(myeqn->rhs() == 7);
                const auto& myeqn_entries = myeqn->entries();
                REQUIRE(myeqn_entries.size() == 2);
                REQUIRE(myeqn_entries.at(y2) == -1);
                REQUIRE(myeqn_entries.at(z3) == 1);
            }
        }
    }

    SECTION("model with free var")
    {
        auto model{jsolve::read_mps(get_mps("example_with_free.mps"))};

        SECTION("original attributes")
        {
            REQUIRE(model.name() == "Unnamed");
            REQUIRE(model.get_constraints().size() == 4);
            REQUIRE(model.get_variables().size() == 4);
        }
    }
}