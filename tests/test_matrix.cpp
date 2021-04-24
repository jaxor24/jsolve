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

TEST_CASE("::operator==", "[matrix]")
{
    Matr m{ 5, 5 , 200.0 };

    SECTION("same matrix")
    {
        REQUIRE(m == m);
    }

    SECTION("different matrix elements")
    {
        Matr m2{ 5, 5 , 100.0 };
        REQUIRE((m == m2) == false);
    }

    SECTION("different matrix dimensions")
    {
        {
            Matr m2{ 4, 5 , 200.0 };
            REQUIRE((m == m2) == false);
        }

        {
            Matr m2{ 5, 4 , 200.0 };
            REQUIRE((m == m2) == false);
        }
    }
}

TEST_CASE("::operator!=", "[matrix]")
{
    Matr m{ 5, 5 , 200.0 };

    SECTION("same matrix")
    {
        REQUIRE((m != m) == false);
    }

    SECTION("different matrix elements")
    {
        Matr m2{ 5, 5 , 100.0 };
        REQUIRE(m != m2);
    }

    SECTION("different matrix dimensions")
    {
        {
            Matr m2{ 4, 5 , 200.0 };
            REQUIRE(m != m2);
        }

        {
            Matr m2{ 5, 4 , 200.0 };
            REQUIRE(m != m2);
        }
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

TEST_CASE("::operator+=", "[matrix]")
{
    SECTION("matrix += matrix")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("valid addition")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        Matr rhs{ r, c, double(r - c) };

                        lhs += rhs;

                        for (auto n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (auto n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) + (r - c));
                            }
                        }
                    }
                }
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) += Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) += Matr(6, 3), MatrixError);
        }
    }

    SECTION("matrix += double")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("valid addition")
                    {
                        Matr lhs{ r, c, double(r + c) };

                        lhs += double(r * c);

                        for (auto n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (auto n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) + (r * c));
                            }
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("::operator+", "[matrix]")
{
    SECTION("matrix + matrix")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };
                    Matr rhs{ r, c, double(r - c) };

                    REQUIRE_NOTHROW(lhs + rhs);

                    Matr result = lhs + rhs;

                    for (auto n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (auto n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) + rhs(n_r, n_c));
                        }
                    }
                }
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) + Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) + Matr(6, 3), MatrixError);
        }
    }

    SECTION("matrix + double")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };

                    Matr result = lhs + double(r * c);

                    for (auto n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (auto n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) + double(r * c));
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("::operator-=", "[matrix]")
{
    SECTION("matrix -= matrix")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("valid addition")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        Matr rhs{ r, c, double(r - c) };

                        lhs -= rhs;

                        for (auto n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (auto n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) - (r - c));
                            }
                        }
                    }
                }
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) -= Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) -= Matr(6, 3), MatrixError);
        }
    }

    SECTION("matrix -= double")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("valid addition")
                    {
                        Matr lhs{ r, c, double(r + c) };

                        lhs -= double(r * c);

                        for (auto n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (auto n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) - (r * c));
                            }
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("::operator-", "[matrix]")
{
    SECTION("matrix - matrix")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };
                    Matr rhs{ r, c, double(r - c) };

                    REQUIRE_NOTHROW(lhs - rhs);

                    Matr result = lhs - rhs;

                    for (auto n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (auto n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) - rhs(n_r, n_c));
                        }
                    }
                }
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) - Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) - Matr(6, 3), MatrixError);
        }
    }

    SECTION("matrix - double")
    {
        SECTION("valid addition")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };

                    Matr result = lhs - double(r * c);

                    for (auto n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (auto n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) - double(r * c));
                        }
                    }
                }
            }
        }
    }
}
