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
        REQUIRE(approx_equal(solution.value().objective, -6.0));
        REQUIRE(approx_equal(solution.value().variables.at("x12"), 1.0));
        REQUIRE(approx_equal(solution.value().variables.at("x13"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x14"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x23"), 1.0));
        REQUIRE(approx_equal(solution.value().variables.at("x24"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x34"), 1.0));
    }

    SECTION("model 9")
    {
        auto model = models::make_model_9();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 8.5);
        REQUIRE(solution.value().variables.at("x1") == 0.0);
        REQUIRE(solution.value().variables.at("x2") == 0.5);
        REQUIRE(solution.value().variables.at("x3") == 0.5);
        REQUIRE(solution.value().variables.at("x4") == 0.5);
        REQUIRE(solution.value().variables.at("x5") == 0.5);
    }

    SECTION("model 10")
    {
        auto model = models::make_model_10();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 13.0);
        REQUIRE(solution.value().variables.at("x1") == 3.0);
        REQUIRE(solution.value().variables.at("x2") == 2.0);
    }

    SECTION("model 11")
    {
        auto model = models::make_model_11();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 1.0);
        REQUIRE(solution.value().variables.at("x1") == 0.0);
        REQUIRE(solution.value().variables.at("x2") == 1.0);
        REQUIRE(solution.value().variables.at("x3") == 0.0);
        REQUIRE(solution.value().variables.at("x4") == 1.0);
    }

    SECTION("model 12")
    {
        auto model = models::make_model_12();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == -25.0);
        REQUIRE(solution.value().variables.at("x1") == 5.0);
        REQUIRE(solution.value().variables.at("x2") == 5.0);
    }

    SECTION("model 13")
    {
        auto model = models::make_model_13();
        auto solution = jsolve::simplex::primal_solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 7.0));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 3.5));
        REQUIRE(approx_equal(solution.value().variables.at("x4"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x5"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x6"), 0.5));
    }
}