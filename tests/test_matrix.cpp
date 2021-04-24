#include "includes.h"

#include "../jsolver/matrix.h"

using Matr = Matrix<double>;

TEST_CASE("constructor 1", "[matrix]")
{
    SECTION("valid construction - normal matrix") 
    {
        auto m = Matr{ 3, 6 };
    }

    SECTION("valid construction - zero dimension matrix")
    {
        auto m = Matr{ 0, 0 };
    }
}

TEST_CASE("constructor 1", "[matrix]")
{
    SECTION("valid construction - normal matrix")
    {
        auto m = Matr{ 3, 6, 10.0 };
    }

    SECTION("valid construction - zero dimension matrix")
    {
        auto m = Matr{ 0, 0, 10.0 };
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
        unsigned n_rows{ 0 };
        unsigned n_cols{ 5 };

        auto m = Matr{ n_rows, n_cols };

        REQUIRE(m.n_rows() == 1);
    }
}
