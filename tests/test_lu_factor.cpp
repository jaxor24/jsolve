#include "test_includes.h"

#include "lu_factor.h"
#include "solve_error.h"

using Matr = Matrix<double>;

TEST_CASE("lu_factor")
{
    SECTION("no pivoting")
    {
        SECTION("1x1 system")
        {
            Matr A{1, 1, 1};

            auto [L, U, perm] = jsolve::lu_factor(A);

            REQUIRE(L(0, 0) == 1);
            REQUIRE(U(0, 0) == 1);
            REQUIRE(perm.size() == 1);
            REQUIRE(perm.at(0) == 0);
        }

        SECTION("2x2 matrix")
        {
            Matr A{2, 2, 0.0};
            A(0, 0) = 6;
            A(0, 1) = 4;
            A(1, 0) = -3;
            A(1, 1) = -1;

            auto [L, U, perm] = jsolve::lu_factor(A);

            REQUIRE(L(0, 0) == 1);
            REQUIRE(L(0, 1) == 0);
            REQUIRE(L(1, 0) == -0.5);
            REQUIRE(L(1, 1) == 1);

            REQUIRE(U(0, 0) == 6);
            REQUIRE(U(0, 1) == 4);
            REQUIRE(U(1, 0) == 0);
            REQUIRE(U(1, 1) == 1);

            REQUIRE(perm.size() == 2);
            REQUIRE(perm.at(0) == 0);
            REQUIRE(perm.at(1) == 1);
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

            auto [L, U, perm] = jsolve::lu_factor(A);

            REQUIRE(L(0, 0) == 1);
            REQUIRE(L(0, 1) == 0);
            REQUIRE(L(0, 2) == 0);
            REQUIRE(L(1, 0) == 1);
            REQUIRE(L(1, 1) == 1);
            REQUIRE(L(1, 2) == 0);
            REQUIRE(L(2, 0) == -0.5);
            REQUIRE(L(2, 1) == -0.25);
            REQUIRE(L(2, 2) == 1);

            REQUIRE(U(0, 0) == -4);
            REQUIRE(U(0, 1) == 6);
            REQUIRE(U(0, 2) == 3);
            REQUIRE(U(1, 0) == 0);
            REQUIRE(U(1, 1) == -8);
            REQUIRE(U(1, 2) == 5);
            REQUIRE(U(2, 0) == 0);
            REQUIRE(U(2, 1) == 0);
            REQUIRE(U(2, 2) == 0.75);

            REQUIRE(perm.size() == 3);
            REQUIRE(perm.at(0) == 1);
            REQUIRE(perm.at(1) == 2);
            REQUIRE(perm.at(2) == 0);
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

            auto [L, U, perm] = jsolve::lu_factor(A);

            REQUIRE(L(0, 0) == 1);
            REQUIRE(L(0, 1) == 0);
            REQUIRE(L(1, 0) == 0);
            REQUIRE(L(1, 1) == 1);

            REQUIRE(U(0, 0) == 2);
            REQUIRE(U(0, 1) == 1);
            REQUIRE(U(1, 0) == 0);
            REQUIRE(U(1, 1) == 1);

            REQUIRE(perm.size() == 2);
            REQUIRE(perm.at(0) == 1);
            REQUIRE(perm.at(1) == 0);
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

            auto [L, U, perm] = jsolve::lu_factor(A);

            REQUIRE(L(0, 0) == 1);
            REQUIRE(L(0, 1) == 0);
            REQUIRE(L(0, 2) == 0);
            REQUIRE(L(1, 0) == 0.75);
            REQUIRE(L(1, 1) == 1);
            REQUIRE(L(1, 2) == 0);
            REQUIRE(L(2, 0) == 0.25);
            REQUIRE(approx_equal(L(2, 1), 0.360583, 1e-6));
            REQUIRE(L(2, 2) == 1);

            REQUIRE(U(0, 0) == 4);
            REQUIRE(U(0, 1) == 235);
            REQUIRE(U(0, 2) == 7);
            REQUIRE(U(1, 0) == 0);
            REQUIRE(U(1, 1) == -171.25);
            REQUIRE(U(1, 2) == -11.25);
            REQUIRE(U(2, 0) == 0);
            REQUIRE(U(2, 1) == 0);
            REQUIRE(approx_equal(U(2, 2), 24.306569, 1e-6));

            REQUIRE(perm.size() == 3);
            REQUIRE(perm.at(0) == 2);
            REQUIRE(perm.at(1) == 1);
            REQUIRE(perm.at(2) == 0);
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

            auto [L, U, perm] = jsolve::lu_factor(A);

            // Diagonal
            REQUIRE(L(0, 0) == 1);
            REQUIRE(L(1, 1) == 1);
            REQUIRE(L(2, 2) == 1);
            REQUIRE(L(3, 3) == 1);
            REQUIRE(L(4, 4) == 1);
            REQUIRE(L(5, 5) == 1);
            REQUIRE(L(6, 6) == 1);
            REQUIRE(L(7, 7) == 1);

            // Lower triangle
            REQUIRE(L(5, 1) == -1);
            REQUIRE(L(6, 1) == 1);
            REQUIRE(L(7, 1) == -1);
            REQUIRE(L(6, 4) == -0.5);
            REQUIRE(L(7, 4) == 0.5);
            REQUIRE(L(7, 6) == -1);

            // Diagonal
            REQUIRE(U(0, 0) == 1);
            REQUIRE(U(1, 1) == -1);
            REQUIRE(U(2, 2) == 1);
            REQUIRE(U(3, 3) == 1);
            REQUIRE(U(4, 4) == 2);
            REQUIRE(U(5, 5) == 1);
            REQUIRE(U(6, 6) == -2.5);
            REQUIRE(U(7, 7) == 1);

            // Upper triangle
            REQUIRE(U(0, 4) == -2);
            REQUIRE(U(1, 4) == -1);
            REQUIRE(U(2, 4) == 2);
            REQUIRE(U(3, 4) == -2);

            REQUIRE(U(0, 6) == 1);
            REQUIRE(U(2, 6) == -1);
            REQUIRE(U(3, 6) == 1);
            REQUIRE(U(4, 6) == -1);

            REQUIRE(perm.size() == 8);
            REQUIRE(perm.at(0) == 0);
            REQUIRE(perm.at(1) == 4);
            REQUIRE(perm.at(2) == 2);
            REQUIRE(perm.at(3) == 3);
            REQUIRE(perm.at(4) == 1);
            REQUIRE(perm.at(5) == 5);
            REQUIRE(perm.at(6) == 6);
            REQUIRE(perm.at(7) == 7);
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

    SECTION("5x5 matrix")
    {
        Matr U{5, 5, 0.0};

        U(0, 0) = -1;
        U(0, 3) = -6;

        U(1, 1) = 1;
        U(1, 3) = 4;

        U(2, 2) = -1;
        U(2, 3) = 2;

        U(3, 3) = -14;

        U(4, 4) = 1;

        Matr b{5, 1, 0.0};

        b(0, 0) = 3;
        b(0, 1) = 0;
        b(0, 2) = 0;
        b(0, 3) = 7;
        b(0, 4) = 1;

        auto result = jsolve::backward_subs(U, b, {1, 2, 0, 4, 3});

        REQUIRE(result.n_rows() == 5);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == -1);
        REQUIRE(result(0, 1) == 0);
        REQUIRE(result(0, 2) == 2);
        REQUIRE(result(0, 3) == 1);
        REQUIRE(result(0, 4) == -0.5);
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

    SECTION("5x5 matrix")
    {
        Matr L{5, 5, 0.0};

        L(0, 0) = 1;

        L(1, 1) = 1;

        L(2, 1) = -1;
        L(2, 2) = 1;

        L(3, 1) = 4;
        L(3, 2) = -2;
        L(3, 3) = 1;

        L(4, 0) = -1;
        L(4, 2) = -3;
        L(4, 4) = 1;

        Matr b{5, 1, 0.0};

        b(0, 0) = 7;
        b(0, 1) = -2;
        b(0, 2) = 0;
        b(0, 3) = 3;
        b(0, 4) = 0;

        auto result = jsolve::forward_subs(L, b, {3, 4, 2, 0, 1});

        REQUIRE(result.n_rows() == 5);
        REQUIRE(result.n_cols() == 1);

        REQUIRE(result(0, 0) == 3);
        REQUIRE(result(0, 1) == 0);
        REQUIRE(result(0, 2) == 0);
        REQUIRE(result(0, 3) == 7);
        REQUIRE(result(0, 4) == 1);
    }
}