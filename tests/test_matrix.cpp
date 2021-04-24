#include "includes.h"

#include "../jsolver/matrix.h"

using Matr = Matrix<double>;

TEST_CASE("::matrix(row,col)", "[matrix]")
{
    SECTION("construction - invalid")
    {
        REQUIRE_THROWS_AS(Matr( 0, 0 ), MatrixError);
    }

    SECTION("construction - valid")
    {
        REQUIRE_NOTHROW(Matr{ 3, 6 });
        REQUIRE_NOTHROW(Matr{ 20, 1 });
        REQUIRE_NOTHROW(Matr{ 1, 500 });
        REQUIRE_NOTHROW(Matr{ 1, 1 });
        REQUIRE_NOTHROW(Matr{ 3, 3 });
    }
}

TEST_CASE("::matrix(row,col,val)", "[matrix]")
{
    SECTION("construction - valid")
    {
        REQUIRE_NOTHROW(Matr{ 3, 6, 10.0 });
        REQUIRE_NOTHROW(Matr{ 1, 1, -500 });
        REQUIRE_NOTHROW(Matr{ 100, 100, -500 });
    }

    SECTION("construction - invalid")
    {
        REQUIRE_THROWS_AS(Matr( 0, 0, 10.0 ), MatrixError);
    }
}

TEST_CASE("::n_rows, ::n_cols", "[matrix]")
{
    SECTION("non-zero rows")
    {
        unsigned n_rows{ 10 };
        unsigned n_cols{ 5 };

        auto m = Matr{ n_rows, n_cols };

        REQUIRE(m.n_rows() == n_rows);
        REQUIRE(m.n_cols() == n_cols);
    }

    SECTION("zero rows")
    {
        unsigned n_rows{ 1 };
        unsigned n_cols{ 5 };

        auto m = Matr{ n_rows, n_cols };

        REQUIRE(m.n_rows() == n_rows);
        REQUIRE(m.n_cols() == n_cols);
    }
}

TEST_CASE("::operator()", "[matrix]")
{
    Matr m{ 10, 5 };

    SECTION("access every element")
    {
        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == 0.0);
            }
        }
    }

    SECTION("assign every element")
    {
        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE_NOTHROW(m(r, c) = 1234);
                REQUIRE(m(r, c) == 1234);
            }
        }
    }

    SECTION("access out of bound row")
    {
        REQUIRE_THROWS_AS(m(m.n_rows(), m.n_cols() - 1), MatrixError);
        REQUIRE_THROWS_AS(m(m.n_rows() + 1, m.n_cols() - 1), MatrixError);
    }

    SECTION("access out of bound col")
    {
        REQUIRE_THROWS_AS(m(m.n_rows() - 1, m.n_cols()), MatrixError);
        REQUIRE_THROWS_AS(m(m.n_rows() - 1, m.n_cols() + 1), MatrixError);
    }
}


TEST_CASE("::operator++", "[matrix]")
{
    Matr m{ 2, 3 };

    SECTION("prefix")
    {
        int n_times{ 100 };

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(++m);
        }
        
        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == n_times);
            }
        }
    }

    SECTION("postfix")
    {
        int n_times{ 100 };

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(m++);
        }

        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == n_times);
            }
        }
    }
}

TEST_CASE("::operator--", "[matrix]")
{
    Matr m{ 2, 3 };

    SECTION("prefix")
    {
        int n_times{ 100 };

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(--m);
        }

        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == -n_times);
            }
        }
    }

    SECTION("postfix")
    {
        int n_times{ 100 };

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(m--);
        }

        for (auto r = 0; r < m.n_rows(); r++)
        {
            for (auto c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == -n_times);
            }
        }
    }
}