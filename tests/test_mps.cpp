#include "test_includes.h"

#include "mps.h"

#include <filesystem>
#include <source_location>

namespace
{
std::filesystem::path get_mps(std::string file_name)
{
    return std::filesystem::path{std::source_location::current().file_name()}.remove_filename() / "mps_examples" /
           file_name;
}
} // namespace

TEST_CASE("jsolve::mps::read_mps", "[matrix]")
{
    SECTION("model 1")
    {
        auto model{jsolve::read_mps(get_mps("example1.mps"))};

        SECTION("attributes")
        {
            REQUIRE(model.name() == "TESTPROB");
            REQUIRE(model.get_constraints().size() == 6);
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
                const auto& lim1_entries = lim1->get_entries();
                REQUIRE(lim1_entries.size() == 2);
                REQUIRE(lim1_entries.at(x1) == 1);
                REQUIRE(lim1_entries.at(y2) == 1);
            }

            {
                const auto* lim2 = model.get_constraint("LIM2");
                REQUIRE(lim2->type() == jsolve::Constraint::Type::GREAT);
                REQUIRE(lim2->rhs() == 10);
                const auto& lim2_entries = lim2->get_entries();
                REQUIRE(lim2_entries.size() == 2);
                REQUIRE(lim2_entries.at(x1) == 1);
                REQUIRE(lim2_entries.at(z3) == 1);
            }

            {
                const auto* myeqn = model.get_constraint("MYEQN");
                REQUIRE(myeqn->type() == jsolve::Constraint::Type::EQUAL);
                REQUIRE(myeqn->rhs() == 7);
                const auto& myeqn_entries = myeqn->get_entries();
                REQUIRE(myeqn_entries.size() == 2);
                REQUIRE(myeqn_entries.at(y2) == -1);
                REQUIRE(myeqn_entries.at(z3) == 1);
            }
        }

        SECTION("bounds as constraints")
        {
            {
                auto* x1_upper_bnd = model.get_constraint("BND_XONE_LEQ_4");
                REQUIRE(x1_upper_bnd->type() == jsolve::Constraint::Type::LESS);
                REQUIRE(x1_upper_bnd->rhs() == 4);
                const auto& x1_upper_bnd_entries = x1_upper_bnd->get_entries();
                REQUIRE(x1_upper_bnd_entries.size() == 1);
                REQUIRE(x1_upper_bnd_entries.at(x1) == 1);
            }

            {
                auto* y2_upper_bnd = model.get_constraint("BND_YTWO_LEQ_1");
                REQUIRE(y2_upper_bnd->type() == jsolve::Constraint::Type::LESS);
                REQUIRE(y2_upper_bnd->rhs() == 1);
                const auto& y2_upper_bnd_entries = y2_upper_bnd->get_entries();
                REQUIRE(y2_upper_bnd_entries.size() == 1);
                REQUIRE(y2_upper_bnd_entries.at(y2) == 1);
            }

            {
                auto* y2_lower_bnd = model.get_constraint("BND_YTWO_GEQ_1");
                REQUIRE(y2_lower_bnd->type() == jsolve::Constraint::Type::GREAT);
                REQUIRE(y2_lower_bnd->rhs() == 1);
                const auto& y2_lower_bnd_entries = y2_lower_bnd->get_entries();
                REQUIRE(y2_lower_bnd_entries.size() == 1);
                REQUIRE(y2_lower_bnd_entries.at(y2) == 1);
            }
        }
    }

    SECTION("model with free var")
    {
        auto model{jsolve::read_mps(get_mps("example_with_free.mps"))};

        SECTION("attributes")
        {
            REQUIRE(model.name() == "Unnamed");
            REQUIRE(model.get_constraints().size() == 6);
            REQUIRE(model.get_variables().size() == 6);
        }

        auto* x1 = model.get_variable("x1");
        auto* x2p = model.get_variable("FREE_x2_POS");
        auto* x2n = model.get_variable("FREE_x2_NEG");
        auto* x3 = model.get_variable("x3");
        auto* x4p = model.get_variable("FREE_x4_POS");
        auto* x4n = model.get_variable("FREE_x4_NEG");

        SECTION("variables")
        {
            REQUIRE(x1->cost() == -1);
            REQUIRE(x2p->cost() == -2);
            REQUIRE(x2n->cost() == 2);
            REQUIRE(x3->cost() == 4);
            REQUIRE(x4p->cost() == 3);
            REQUIRE(x4n->cost() == -3);

            REQUIRE(x1->lower_bound() == 0);
            REQUIRE(x2p->lower_bound() == 0);
            REQUIRE(x2n->lower_bound() == 0);
            REQUIRE(x3->lower_bound() == 1.1);
            REQUIRE(x4p->lower_bound() == 0);
            REQUIRE(x4n->lower_bound() == 0);

            REQUIRE(x1->upper_bound() == std::numeric_limits<double>::infinity());
            REQUIRE(x2p->upper_bound() == std::numeric_limits<double>::infinity());
            REQUIRE(x2n->upper_bound() == std::numeric_limits<double>::infinity());
            REQUIRE(x3->upper_bound() == 10);
            REQUIRE(x4p->upper_bound() == std::numeric_limits<double>::infinity());
            REQUIRE(x4n->upper_bound() == std::numeric_limits<double>::infinity());
        }
    }
}