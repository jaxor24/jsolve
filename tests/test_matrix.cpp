#include "includes.h"

#include "../jsolver/matrix.h"

using Matr = Matrix<double>;

TEST_CASE("::matrix(row,col)", "[matrix]")
{
    SECTION("construction - zero dimension matrix")
    {
        REQUIRE_THROWS_AS(Matr( 0, 0 ), MatrixError);
    }

    SECTION("construction - non-square matrix")
    {
        REQUIRE_NOTHROW(Matr{ 3, 6 });
        REQUIRE_NOTHROW(Matr{ 20, 1 });
        REQUIRE_NOTHROW(Matr{ 1, 500 });
    }

    SECTION("construction - square matrix")
    {
        REQUIRE_NOTHROW(Matr{ 1, 1 });
        REQUIRE_NOTHROW(Matr{ 3, 3 });
    }
}

TEST_CASE("::matrix(row,col,val)", "[matrix]")
{
    SECTION("construction - normal matrix")
    {
        REQUIRE_NOTHROW(Matr{ 3, 6, 10.0 });
    }

    SECTION("construction - zero dimension matrix")
    {
        REQUIRE_THROWS_AS(Matr( 0, 0, 10.0 ), MatrixError);
    }
}

TEST_CASE("::n_rows", "[matrix]")
{
    SECTION("non-zero rows")
    {
        unsigned n_rows{ 10 };
        unsigned n_cols{ 5 };

        auto m = Matr{ n_rows, n_cols };

        REQUIRE(m.n_rows() == n_rows);
    }

    SECTION("zero rows")
    {
        unsigned n_rows{ 1 };
        unsigned n_cols{ 5 };

        auto m = Matr{ n_rows, n_cols };

        REQUIRE(m.n_rows() == n_rows);
    }
}
