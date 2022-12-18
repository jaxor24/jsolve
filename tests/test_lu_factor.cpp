#include "test_includes.h"

#include "lu_factor.h"
#include "solve_error.h"

using Matr = Matrix<double>;

TEST_CASE("lu_refactor")
{
    SECTION("1x1 system")
    {
        Matr A{1, 1, 5};
        Matr b{1, 1, 10};
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
    }

    SECTION("3x3 matrix a")
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

        auto [L, U] = jsolve::lu_refactor(A);

        REQUIRE(L.n_rows() == A.n_rows());
        REQUIRE(L.n_cols() == A.n_cols());
        REQUIRE(U.n_cols() == A.n_rows());
        REQUIRE(U.n_cols() == A.n_cols());

        REQUIRE(L(0, 0) == 1);
        REQUIRE(L(0, 1) == 0);
        REQUIRE(L(0, 2) == 0);

        REQUIRE(L(1, 0) == -2);
        REQUIRE(L(1, 1) == 1);
        REQUIRE(L(1, 2) == 0);

        REQUIRE(L(2, 0) == -2);
        REQUIRE(L(2, 1) == -1);
        REQUIRE(L(2, 2) == 1);

        REQUIRE(U(0, 0) == 2);
        REQUIRE(U(0, 1) == -1);
        REQUIRE(U(0, 2) == -2);

        REQUIRE(U(1, 0) == 0);
        REQUIRE(U(1, 1) == 4);
        REQUIRE(U(1, 2) == -1);

        REQUIRE(U(2, 0) == 0);
        REQUIRE(U(2, 1) == 0);
        REQUIRE(U(2, 2) == 3);
    }
}