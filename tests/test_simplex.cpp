#include "test_includes.h"

#include "models.h"

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
}