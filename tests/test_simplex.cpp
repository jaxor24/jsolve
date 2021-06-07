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
    }

    SECTION("model 3")
    {
        auto model = models::make_model_3();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 17.0);
    }
}