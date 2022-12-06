#include "test_includes.h"

#include "solve_error.h"
#include "solve_gauss.h"

using Matr = Matrix<double>;

TEST_CASE("solve_gauss")
{
    SECTION("1x1 system")
    {
        Matr A{1, 1, 5};
        Matr b{1, 1, 10};

        auto result = jsolve::solve_gauss(A, b);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);
        REQUIRE(result(0, 0) == 2);
    }

    SECTION("2x2 system")
    {
        Matr A{2, 2, 0.0};
        A(0, 0) = 1;
        A(0, 1) = 2;
        A(1, 0) = -3;
        A(1, 1) = -4;

        Matr b{2, 1, 0.0};
        b(0, 0) = 6;
        b(0, 1) = -5;

        auto result = jsolve::solve_gauss(A, b);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == -7.0);
        REQUIRE(result(0, 1) == 6.5);
    }

    SECTION("3x3 matrix")
    {
        Matr A{3, 3, 0.0};
        A(0, 0) = 1;
        A(0, 1) = 1;
        A(0, 2) = 1;
        A(1, 0) = 0;
        A(1, 1) = 2;
        A(1, 2) = 5;
        A(2, 0) = 2;
        A(2, 1) = 5;
        A(2, 2) = -1;

        Matr b{3, 1, 0.0};
        b(0, 0) = 6;
        b(0, 1) = -4;
        b(0, 2) = 27;

        auto result = jsolve::solve_gauss(A, b);

        REQUIRE(result.n_rows() == 3);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 5);
        REQUIRE(result(0, 1) == 3);
        REQUIRE(result(0, 2) == -2);
    }

    SECTION("invalid input A")
    {
        REQUIRE_THROWS_AS(jsolve::solve_gauss(Matr{10, 1}, Matr{1, 1}), jsolve::SolveError);
    }

    SECTION("invalid input b")
    {
        REQUIRE_THROWS_AS(jsolve::solve_gauss(Matr{1, 1}, Matr{1, 123}), jsolve::SolveError);
    }

    SECTION("inconsistent inputs A and b")
    {
        REQUIRE_THROWS_AS(jsolve::solve_gauss(Matr{1, 10}, Matr{11, 1}), jsolve::SolveError);
    }
}
