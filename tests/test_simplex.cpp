#include "test_includes.h"

#include "models.h"
#include "tools.h"

TEST_CASE("jsolve::simplex::primal_solve", "[matrix]")
{
    SECTION("model 1")
    {
        auto model = models::make_model_1();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 13.0);
        REQUIRE(solution.value().variables.at("x1") == 2.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
        REQUIRE(solution.value().variables.at("x3") == 1.0);
    }

    SECTION("model 2")
    {
        auto model = models::make_model_2();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, -3.0));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 4.0 / 3.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 1.0 / 3.0));
    }

    SECTION("model 3")
    {
        auto model = models::make_model_3();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 17.0);
        REQUIRE(solution.value().variables.at("x1") == 2.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
        REQUIRE(solution.value().variables.at("x3") == 1.0);
        REQUIRE(solution.value().variables.at("x4") == 0.0);
    }

    SECTION("model 4")
    {
        auto model = models::make_model_4();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 2.0);
        REQUIRE(solution.value().variables.at("x1") == 1.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
    }

    SECTION("model 6")
    {
        auto model = models::make_model_6();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 28.0);
        REQUIRE(solution.value().variables.at("x1") == 4.0);
        REQUIRE(solution.value().variables.at("x2") == 8.0);
    }

    SECTION("model 7")
    {
        auto model = models::make_model_7();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 9.0);
        REQUIRE(solution.value().variables.at("x1") == 0.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
        REQUIRE(solution.value().variables.at("x3") == 0.0);
        REQUIRE(solution.value().variables.at("x4") == 1.0);
    }

    SECTION("model 8")
    {
        auto model = models::make_model_8();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 9.0);
        REQUIRE(solution.value().variables.at("x12") == 1.0);
        REQUIRE(solution.value().variables.at("x13") == 0.0);
        REQUIRE(solution.value().variables.at("x14") == 0.0);
        REQUIRE(solution.value().variables.at("x23") == 0.0);
        REQUIRE(solution.value().variables.at("x24") == 1.0);
        REQUIRE(solution.value().variables.at("x34") == 0.0);
    }
}