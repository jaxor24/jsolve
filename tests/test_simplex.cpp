#include "test_includes.h"

#include "models.h"

TEST_CASE("jsolve::simplex::primal_solve", "[matrix]")
{
    SECTION("feasible - model 1")
    {
        SECTION("model 1")
        {
            auto model = models::make_model_1();
            auto solution = jsolve::simplex::primal_solve(model);

            REQUIRE(solution.has_value());
            REQUIRE(solution.value().objective == 13.0);
        }
    }
}