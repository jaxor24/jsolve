#include "test_includes.h"

#include "models.h"
#include "tools.h"

TEST_CASE("jsolve::primal_solve", "[matrix]")
{
    SECTION("model 1")
    {
        auto model = models::make_model_1();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 13.0);
        REQUIRE(solution.value().variables.at("x1") == 2.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
        REQUIRE(solution.value().variables.at("x3") == 1.0);
    }

    SECTION("model 2")
    {
        auto model = models::make_model_2();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, -3.0));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 4.0 / 3.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 1.0 / 3.0));
    }

    SECTION("model 3")
    {
        auto model = models::make_model_3();
        auto solution = jsolve::solve(model);

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
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 2.0);
        REQUIRE(solution.value().variables.at("x1") == 1.0);
        REQUIRE(solution.value().variables.at("x2") == 0.0);
    }

    SECTION("model 6")
    {
        auto model = models::make_model_6();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 28.0);
        REQUIRE(solution.value().variables.at("x1") == 4.0);
        REQUIRE(solution.value().variables.at("x2") == 8.0);
    }

    SECTION("model 7")
    {
        auto model = models::make_model_7();
        auto solution = jsolve::solve(model);

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
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 6.0));
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
        auto solution = jsolve::solve(model);

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
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 13.0);
        REQUIRE(solution.value().variables.at("x1") == 3.0);
        REQUIRE(solution.value().variables.at("x2") == 2.0);
    }

    SECTION("model 11")
    {
        auto model = models::make_model_11();
        auto solution = jsolve::solve(model);

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
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(solution.value().objective == 25.0);
        REQUIRE(solution.value().variables.at("x1") == 5.0);
        REQUIRE(solution.value().variables.at("x2") == 5.0);
    }

    SECTION("model 13")
    {
        auto model = models::make_model_13();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 7.0));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 3.5));
        REQUIRE(approx_equal(solution.value().variables.at("x4"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x5"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x6"), 0.5));
    }

    SECTION("model 14")
    {
        auto model = models::make_model_14();
        auto solution = jsolve::solve(model);

        REQUIRE(!solution.has_value());
    }

    SECTION("model 15")
    {
        auto model = models::make_model_15();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 5.4));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0.2));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 1.6));
    }

    SECTION("model 16")
    {
        auto model = models::make_model_16();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 2.2));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 0.4));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 1.8));
    }

    SECTION("model 17")
    {
        auto model = models::make_model_17();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 42));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0.0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 10.4));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 0));
        REQUIRE(approx_equal(solution.value().variables.at("x4"), 0.4));
    }

    SECTION("model 18")
    {
        auto model = models::make_model_18();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 51 + (3.0 / 7.0)));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 6 + (3.0 / 7.0)));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 4 + (2.0 / 7.0)));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 0));
    }

    SECTION("model 19")
    {
        auto model = models::make_model_19();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 1.5));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 2.5));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 1.5));
        REQUIRE(approx_equal(solution.value().variables.at("x4"), 0));
    }

    SECTION("model 20")
    {
        auto model = models::make_model_20();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 11));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 4));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 1));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 2));
    }

    SECTION("model 21")
    {
        auto model = models::make_model_21();
        auto solution = jsolve::solve(model);

        REQUIRE(!solution.has_value());
    }

    SECTION("model 22")
    {
        auto model = models::make_model_22();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
    }

    SECTION("model 23")
    {
        auto model = models::make_model_23();
        auto solution = jsolve::solve(model);

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 25));
        REQUIRE(approx_equal(solution.value().variables.at("x1"), 0));
        REQUIRE(approx_equal(solution.value().variables.at("x2"), 0.5));
        REQUIRE(approx_equal(solution.value().variables.at("x3"), 0.5));
    }

    SECTION("model 24")
    {
        auto model = models::make_model_24();
        auto solution = jsolve::solve(model);

        double eps = 0.0001;

        REQUIRE(solution.has_value());
        REQUIRE(approx_equal(solution.value().objective, 61.6562, eps));
        REQUIRE(approx_equal(solution.value().variables.at("a1"), 54.4253, eps));
        REQUIRE(approx_equal(solution.value().variables.at("a2"), 15.6023, eps));
        REQUIRE(approx_equal(solution.value().variables.at("a3"), 24.9724, eps));
        REQUIRE(approx_equal(solution.value().variables.at("a4"), 0.0, eps));

        REQUIRE(approx_equal(solution.value().variables.at("b1"), 0.574694, eps));
        REQUIRE(approx_equal(solution.value().variables.at("b2"), 14.9803, eps));
        REQUIRE(approx_equal(solution.value().variables.at("b3"), 20.0000, eps));
        REQUIRE(approx_equal(solution.value().variables.at("b4"), 27.4450, eps));

        REQUIRE(approx_equal(solution.value().variables.at("c1"), 0.0, eps));
        REQUIRE(approx_equal(solution.value().variables.at("c2"), 42.4174, eps));
        REQUIRE(approx_equal(solution.value().variables.at("c3"), 60.0276, eps));
        REQUIRE(approx_equal(solution.value().variables.at("c4"), 10.5550, eps));

        REQUIRE(approx_equal(solution.value().variables.at("pmax"), 11.4, eps));
    }
}
