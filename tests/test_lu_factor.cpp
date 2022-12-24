#include "test_includes.h"

#include "lu_factor.h"
#include "solve_error.h"

using Matr = Matrix<double>;

TEST_CASE("lu_refactor")
{
    SECTION("no pivoting")
    {
        SECTION("1x1 system")
        {
            Matr A{1, 1, 5};

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }

        SECTION("2x2 matrix")
        {
            Matr A{2, 2, 0.0};
            A(0, 0) = 0;
            A(0, 1) = 1;
            A(1, 0) = 2;
            A(1, 1) = 1;

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }

        SECTION("3x3 matrix")
        {
            Matr A{4, 4, 0.0};
            A(0, 0) = 4;
            A(0, 1) = 3;
            A(0, 2) = 4;
            A(0, 3) = 2;

            A(1, 0) = 1;
            A(1, 1) = 2;
            A(1, 2) = 2;
            A(1, 3) = 4;

            A(2, 0) = 8;
            A(2, 1) = 9;
            A(2, 2) = 9;
            A(2, 3) = 1;

            A(3, 0) = 6;
            A(3, 1) = 4;
            A(3, 2) = 8;
            A(3, 3) = 8;

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }

        SECTION("3x3 matrix")
        {
            Matr A{3, 3, 0.0};
            A(0, 0) = 2;
            A(0, 1) = -1;
            A(0, 2) = -2;

            A(1, 0) = -4;
            A(1, 1) = 6;
            A(1, 2) = 3;

            A(2, 0) = -4;
            A(2, 1) = -2;
            A(2, 2) = 8;

            auto [L, U, perm] = jsolve::lu_refactor(A);

            REQUIRE(L.n_rows() == A.n_rows());
            REQUIRE(L.n_cols() == A.n_cols());
            REQUIRE(U.n_cols() == A.n_rows());
            REQUIRE(U.n_cols() == A.n_cols());

            REQUIRE(A == L * U);
        }

        SECTION("5x5 matrix")
        {
            Matr A{5, 5, 0.0};

            A(0, 0) = 2;
            A(0, 2) = 4;
            A(0, 4) = -2;

            A(1, 0) = 3;
            A(1, 1) = 1;
            A(1, 3) = 1;

            A(2, 0) = -1;
            A(2, 2) = -1;
            A(2, 4) = -2;

            A(3, 1) = -1;
            A(3, 4) = -6;

            A(4, 2) = 1;
            A(4, 4) = 4;

            auto [L, U, perm] = jsolve::lu_refactor(A);

            REQUIRE(L.n_rows() == A.n_rows());
            REQUIRE(L.n_cols() == A.n_cols());
            REQUIRE(U.n_cols() == A.n_rows());
            REQUIRE(U.n_cols() == A.n_cols());

            REQUIRE(A == L * U);
        }
    }

    SECTION("pivoting needed")
    {
        SECTION("2x2 matrix")
        {
            Matr A{2, 2, 0.0};
            A(0, 0) = 0;
            A(0, 1) = 1;
            A(1, 0) = 2;
            A(1, 1) = 1;

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }

        SECTION("3x3 matrix")
        {
            Matr A{3, 3, 0.0};
            A(0, 0) = 1;
            A(0, 1) = -3;
            A(0, 2) = 22;

            A(1, 0) = 3;
            A(1, 1) = 5;
            A(1, 2) = -6;

            A(2, 0) = 4;
            A(2, 1) = 235;
            A(2, 2) = 7;

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }

        SECTION("8x8 matrix")
        {
            Matr A{8, 8, 0.0};
            A(0, 0) = 1;
            A(0, 4) = -2;
            A(0, 6) = 1;

            A(1, 4) = 2;
            A(1, 6) = -1;

            A(2, 2) = 1;
            A(2, 4) = 2;
            A(2, 6) = -1;

            A(3, 3) = 1;
            A(3, 4) = -2;
            A(3, 6) = 1;

            A(4, 1) = -1;
            A(4, 4) = -1;

            A(5, 1) = 1;
            A(5, 4) = 1;
            A(5, 5) = 1;

            A(6, 1) = -1;
            A(6, 4) = -2;
            A(6, 6) = -2;

            A(7, 1) = 1;
            A(7, 4) = 2;
            A(7, 6) = 2;
            A(7, 7) = 1;

            auto [L, U, perm] = jsolve::lu_refactor(A);
            REQUIRE(A == L * U);
        }
    }
}

TEST_CASE("backward_subs")
{
    SECTION("1x1 system")
    {
        Matr A{1, 1, 5};
        Matr b{1, 1, 10};

        auto result = jsolve::backward_subs(A, b);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 2);
    }

    SECTION("2x2 system")
    {
        Matr A{2, 2, 0.0};

        A(0, 0) = 1;
        A(0, 1) = 2;
        A(1, 0) = 0;
        A(1, 1) = -10;

        Matr b{2, 1, 0.0};

        b(0, 0) = 6;
        b(0, 1) = -5;

        auto result = jsolve::backward_subs(A, b);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 5.0);
        REQUIRE(result(0, 1) == 0.5);
    }

    SECTION("3x3 matrix")
    {
        Matr A{3, 3, 0.0};

        A(0, 0) = 2;
        A(0, 1) = -1;
        A(0, 2) = -2;

        A(1, 0) = 0;
        A(1, 1) = 4;
        A(1, 2) = -1;

        A(2, 0) = 0;
        A(2, 1) = 0;
        A(2, 2) = 3;

        Matr b{3, 1, 0.0};

        b(0, 0) = -1;
        b(0, 1) = 11;
        b(0, 2) = 3;

        auto result = jsolve::backward_subs(A, b);

        REQUIRE(result.n_rows() == A.n_rows());
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 2);
        REQUIRE(result(0, 1) == 3);
        REQUIRE(result(0, 2) == 1);
    }
}

TEST_CASE("forward_subs")
{
    SECTION("1x1 system")
    {
        Matr A{1, 1, 5};
        Matr b{1, 1, 10};

        auto result = jsolve::forward_subs(A, b);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 2);
    }

    SECTION("2x2 system")
    {
        Matr A{2, 2, 0.0};

        A(0, 0) = -10;
        A(0, 1) = 0;
        A(1, 0) = 1;
        A(1, 1) = 2;

        Matr b{2, 1, 0.0};

        b(0, 0) = -5;
        b(0, 1) = 9;

        auto result = jsolve::forward_subs(A, b);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 0.5);
        REQUIRE(result(0, 1) == 4.25);
    }

    SECTION("5x5 matrix")
    {
        Matr A{5, 5, 0.0};

        A(0, 0) = 1;

        A(1, 0) = 1.5;
        A(1, 1) = 1;

        A(2, 0) = -0.5;
        A(2, 2) = 1;

        A(3, 1) = -1;
        A(3, 2) = -6;
        A(3, 3) = 1;

        A(4, 2) = 1;
        A(4, 4) = 1;

        Matr b{5, 1, 0.0};

        b(0, 0) = 7;
        b(0, 1) = -2;
        b(0, 2) = 0;
        b(0, 3) = 3;
        b(0, 4) = 0;

        auto result = jsolve::forward_subs(A, b);

        REQUIRE(result.n_rows() == A.n_rows());
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 7);
        REQUIRE(result(0, 1) == -25.0 / 2);
        REQUIRE(result(0, 2) == 7.0 / 2);
        REQUIRE(result(0, 3) == 23.0 / 2);
        REQUIRE(result(0, 4) == -7.0 / 2);
    }
}