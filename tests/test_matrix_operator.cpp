#include "test_includes.h"

#include "matrix.h"

#include <stdexcept>

using Matr = Matrix<double>;

TEST_CASE("matrix::operator()", "[matrix]")
{
    Matr m{10, 5};

    SECTION("access every element")
    {
        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == 0.0);
            }
        }
    }

    SECTION("assign every element")
    {
        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE_NOTHROW(m(r, c) = 1234);
                REQUIRE(m(r, c) == 1234);
            }
        }
    }

#ifndef NDEBUG
    SECTION("access out of bound row")
    {
        // Bounds checking only done in debug builds for performance reasons
        REQUIRE_THROWS_AS(m(m.n_rows(), m.n_cols() - 1), std::out_of_range);
        REQUIRE_THROWS_AS(m(m.n_rows() + 1, m.n_cols() - 1), std::out_of_range);
    }

    SECTION("access out of bound col")
    {
        // Bounds checking only done in debug builds for performance reasons
        REQUIRE_THROWS_AS(m(m.n_rows() - 1, m.n_cols()), std::out_of_range);
        REQUIRE_THROWS_AS(m(m.n_rows() - 1, m.n_cols() + 1), std::out_of_range);
    }
#endif
}

TEST_CASE("matrix::operator==", "[matrix]")
{
    Matr m{5, 5, 200.0};

    SECTION("same matrix")
    {
        REQUIRE(m == m);
    }

    SECTION("different matrix elements")
    {
        Matr m2{5, 5, 100.0};
        REQUIRE((m == m2) == false);
    }

    SECTION("different matrix dimensions")
    {
        {
            Matr m2{4, 5, 200.0};
            REQUIRE((m == m2) == false);
        }

        {
            Matr m2{5, 4, 200.0};
            REQUIRE((m == m2) == false);
        }
    }
}

TEST_CASE("matrix::operator!=", "[matrix]")
{
    Matr m{5, 5, 200.0};

    SECTION("same matrix")
    {
        REQUIRE((m != m) == false);
    }

    SECTION("different matrix elements")
    {
        Matr m2{5, 5, 100.0};
        REQUIRE(m != m2);
    }

    SECTION("different matrix dimensions")
    {
        {
            Matr m2{4, 5, 200.0};
            REQUIRE(m != m2);
        }

        {
            Matr m2{5, 4, 200.0};
            REQUIRE(m != m2);
        }
    }
}

TEST_CASE("matrix::operator-", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m1{1, 1, 1.0};

        auto m2 = -m1;

        REQUIRE(m2(0, 0) == -1.0);
    }

    SECTION("2x2 matrix")
    {
        Matr m1{2, 2, 0.0}; // [1 2; 3 4]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        auto m2 = -m1;

        REQUIRE(m2(0, 0) == -1.0);
        REQUIRE(m2(0, 1) == -2.0);
        REQUIRE(m2(1, 0) == -3.0);
        REQUIRE(m2(1, 1) == -4.0);
    }

    SECTION("1x3 vector")
    {
        Matr m1{1, 3}; // [1 2 3]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(0, 2) = 3;

        auto m2 = -m1;

        REQUIRE(m2(0, 0) == -1.0);
        REQUIRE(m2(0, 1) == -2.0);
        REQUIRE(m2(0, 2) == -3.0);
    }
}

TEST_CASE("matrix::operator++", "[matrix]")
{
    Matr m{2, 3};

    SECTION("prefix")
    {
        int n_times{10};

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(++m);
        }

        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == n_times);
            }
        }
    }
}

TEST_CASE("matrix::operator++(int)", "[matrix]")
{
    Matr m{2, 3};

    SECTION("postfix")
    {
        int n_times{10};

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(m++);
        }

        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == n_times);
            }
        }
    }
}

TEST_CASE("matrix::operator--", "[matrix]")
{
    Matr m{2, 3};

    SECTION("prefix")
    {
        int n_times{10};

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(--m);
        }

        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == -n_times);
            }
        }
    }
}

TEST_CASE("matrix::operator--(int)", "[matrix]")
{
    Matr m{2, 3};

    SECTION("postfix")
    {
        int n_times{10};

        for (auto i = 0; i < n_times; i++)
        {
            REQUIRE_NOTHROW(m--);
        }

        for (std::size_t r = 0; r < m.n_rows(); r++)
        {
            for (std::size_t c = 0; c < m.n_cols(); c++)
            {
                REQUIRE(m(r, c) == -n_times);
            }
        }
    }
}

TEST_CASE("matrix::operator+=", "[matrix]")
{
    SECTION("operator+=(Matrix)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};
            Matr m2{1, 1, -5.0};

            m1 += m2;

            REQUIRE(m1(0, 0) == -4);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            Matr m2{2, 2, 0.0}; // [4 3; 2 1]
            m2(0, 0) = 4;
            m2(0, 1) = 3;
            m2(1, 0) = 2;
            m2(1, 1) = 1;

            m1 += m2;

            REQUIRE(m1(0, 0) == 5);
            REQUIRE(m1(0, 1) == 5);
            REQUIRE(m1(1, 0) == 5);
            REQUIRE(m1(1, 1) == 5);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            Matr m2{1, 3}; // [-3 -2 -1]
            m2(0, 0) = -3;
            m2(0, 1) = -2;
            m2(0, 2) = -1;

            m1 += m2;

            REQUIRE(m1(0, 0) == -2);
            REQUIRE(m1(0, 1) == 0);
            REQUIRE(m1(0, 2) == 2);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            Matr m2{3, 1}; // [-3;-2;-1]
            m2(0, 0) = -3;
            m2(1, 0) = -2;
            m2(2, 0) = -1;

            m1 += m2;

            REQUIRE(m1(0, 0) == -2);
            REQUIRE(m1(1, 0) == 0);
            REQUIRE(m1(2, 0) == 2);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) += Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) += Matr(6, 3), MatrixError);
        }
    }

    SECTION("operator+=(double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            m1 += 6;

            REQUIRE(m1(0, 0) == 7);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            m1 += 6;

            REQUIRE(m1(0, 0) == 7);
            REQUIRE(m1(0, 1) == 8);
            REQUIRE(m1(1, 0) == 9);
            REQUIRE(m1(1, 1) == 10);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            m1 += 6;

            REQUIRE(m1(0, 0) == 7);
            REQUIRE(m1(0, 1) == 8);
            REQUIRE(m1(0, 2) == 9);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            m1 += 6;

            REQUIRE(m1(0, 0) == 7);
            REQUIRE(m1(1, 0) == 8);
            REQUIRE(m1(2, 0) == 9);
        }
    }
}

TEST_CASE("matrix::operator-=", "[matrix]")
{
    SECTION("operator-=(Matrix)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};
            Matr m2{1, 1, -5.0};

            m1 -= m2;

            REQUIRE(m1(0, 0) == 6);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            Matr m2{2, 2, 0.0}; // [4 3; 2 1]
            m2(0, 0) = 4;
            m2(0, 1) = 3;
            m2(1, 0) = 2;
            m2(1, 1) = 1;

            m1 -= m2;

            REQUIRE(m1(0, 0) == -3);
            REQUIRE(m1(0, 1) == -1);
            REQUIRE(m1(1, 0) == 1);
            REQUIRE(m1(1, 1) == 3);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            Matr m2{1, 3}; // [-3 -2 -1]
            m2(0, 0) = -3;
            m2(0, 1) = -2;
            m2(0, 2) = -1;

            m1 -= m2;

            REQUIRE(m1(0, 0) == 4);
            REQUIRE(m1(0, 1) == 4);
            REQUIRE(m1(0, 2) == 4);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            Matr m2{3, 1}; // [-3;-2;-1]
            m2(0, 0) = -3;
            m2(1, 0) = -2;
            m2(2, 0) = -1;

            m1 -= m2;

            REQUIRE(m1(0, 0) == 4);
            REQUIRE(m1(1, 0) == 4);
            REQUIRE(m1(2, 0) == 4);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) -= Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) -= Matr(6, 3), MatrixError);
        }
    }

    SECTION("operator-=(double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            m1 -= 6;

            REQUIRE(m1(0, 0) == -5);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            m1 -= 6;

            REQUIRE(m1(0, 0) == -5);
            REQUIRE(m1(0, 1) == -4);
            REQUIRE(m1(1, 0) == -3);
            REQUIRE(m1(1, 1) == -2);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            m1 -= 6;

            REQUIRE(m1(0, 0) == -5);
            REQUIRE(m1(0, 1) == -4);
            REQUIRE(m1(0, 2) == -3);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            m1 -= 6;

            REQUIRE(m1(0, 0) == -5);
            REQUIRE(m1(1, 0) == -4);
            REQUIRE(m1(2, 0) == -3);
        }
    }
}

TEST_CASE("matrix::operator/=", "[matrix]")
{
    SECTION("operator/=(double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 6};

            m1 /= 6;

            REQUIRE(m1(0, 0) == 1);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = -24;
            m1(0, 1) = -18;
            m1(1, 0) = -12;
            m1(1, 1) = -3;

            m1 /= 6;

            REQUIRE(m1(0, 0) == -4);
            REQUIRE(m1(0, 1) == -3);
            REQUIRE(m1(1, 0) == -2);
            REQUIRE(m1(1, 1) == -0.5);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 3;
            m1(0, 1) = 9;
            m1(0, 2) = 18;

            m1 /= 6;

            REQUIRE(m1(0, 0) == 0.5);
            REQUIRE(m1(0, 1) == 1.5);
            REQUIRE(m1(0, 2) == 3);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 3;
            m1(1, 0) = 9;
            m1(2, 0) = 18;

            m1 /= 6;

            REQUIRE(m1(0, 0) == 0.5);
            REQUIRE(m1(1, 0) == 1.5);
            REQUIRE(m1(2, 0) == 3);
        }

        SECTION("invalid - division by zero")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) /= 0, MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) /= 0, MatrixError);
        }
    }
}

TEST_CASE("matrix::operator*=", "[matrix]")
{
    SECTION("operator*=(Matrix)")
    {
        SECTION("[1x1] LHS")
        {
            Matr m1{1, 1, 1.0};

            SECTION("[1x1] RHS")
            {
                Matr m2{1, 1, -5.0};

                m1 *= m2;

                REQUIRE(m1.n_rows() == 1);
                REQUIRE(m1.n_cols() == 1);
                REQUIRE(m1(0, 0) == -5);
            }

            SECTION("[1x4] RHS")
            {
                Matr m2{1, 4, 0.0};
                m2(0, 0) = 1;
                m2(0, 1) = 2;
                m2(0, 2) = 3;
                m2(0, 3) = 4;

                m1 *= m2;

                REQUIRE(m1.n_rows() == 1);
                REQUIRE(m1.n_cols() == 4);
                REQUIRE(m1(0, 0) == 1);
                REQUIRE(m1(0, 1) == 2);
                REQUIRE(m1(0, 2) == 3);
                REQUIRE(m1(0, 3) == 4);
            }
        }

        SECTION("[2x2] LHS")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            SECTION("[2x2] RHS")
            {
                Matr m2{2, 2, 0.0}; // [4 3; 2 1]
                m2(0, 0) = 4;
                m2(0, 1) = 3;
                m2(1, 0) = 2;
                m2(1, 1) = 1;

                m1 *= m2;

                REQUIRE(m1.n_rows() == 2);
                REQUIRE(m1.n_cols() == 2);
                REQUIRE(m1(0, 0) == 8);
                REQUIRE(m1(0, 1) == 5);
                REQUIRE(m1(1, 0) == 20);
                REQUIRE(m1(1, 1) == 13);
            }

            SECTION("[2x1] RHS")
            {
                Matr m2{2, 1, 0.0}; // [-1; -2]
                m2(0, 0) = -1;
                m2(1, 0) = -2;

                m1 *= m2;

                REQUIRE(m1.n_rows() == 2);
                REQUIRE(m1.n_cols() == 1);
                REQUIRE(m1(0, 0) == -5);
                REQUIRE(m1(1, 0) == -11);
            }
        }

        SECTION("[1x3] LHS")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            SECTION("[3x1] RHS")
            {
                Matr m2{3, 1}; // [3;2;1]
                m2(0, 0) = 3;
                m2(1, 0) = 2;
                m2(2, 0) = 1;

                m1 *= m2;

                REQUIRE(m1.n_rows() == 1);
                REQUIRE(m1.n_cols() == 1);
                REQUIRE(m1(0, 0) == 10);
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 2) *= Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(2, 1) *= Matr(2, 1), MatrixError);
        }
    }

    SECTION("operator*=(double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            m1 *= 6;

            REQUIRE(m1(0, 0) == 6);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            m1 *= 6;

            REQUIRE(m1(0, 0) == 6);
            REQUIRE(m1(0, 1) == 12);
            REQUIRE(m1(1, 0) == 18);
            REQUIRE(m1(1, 1) == 24);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            m1 *= 6;

            REQUIRE(m1(0, 0) == 6);
            REQUIRE(m1(0, 1) == 12);
            REQUIRE(m1(0, 2) == 18);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            m1 *= 6;

            REQUIRE(m1(0, 0) == 6);
            REQUIRE(m1(1, 0) == 12);
            REQUIRE(m1(2, 0) == 18);
        }
    }
}

TEST_CASE("operator+", "[matrix]")
{
    SECTION("operator+(Matrix, Matrix)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};
            Matr m2{1, 1, -5.0};

            auto result = m1 + m2;

            REQUIRE(result(0, 0) == -4);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            Matr m2{2, 2, 0.0}; // [4 3; 2 1]
            m2(0, 0) = 4;
            m2(0, 1) = 3;
            m2(1, 0) = 2;
            m2(1, 1) = 1;

            auto result = m1 + m2;

            REQUIRE(result(0, 0) == 5);
            REQUIRE(result(0, 1) == 5);
            REQUIRE(result(1, 0) == 5);
            REQUIRE(result(1, 1) == 5);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            Matr m2{1, 3}; // [-3 -2 -1]
            m2(0, 0) = -3;
            m2(0, 1) = -2;
            m2(0, 2) = -1;

            auto result = m1 + m2;

            REQUIRE(result(0, 0) == -2);
            REQUIRE(result(0, 1) == 0);
            REQUIRE(result(0, 2) == 2);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            Matr m2{3, 1}; // [-3;-2;-1]
            m2(0, 0) = -3;
            m2(1, 0) = -2;
            m2(2, 0) = -1;

            auto result = m1 + m2;

            REQUIRE(result(0, 0) == -2);
            REQUIRE(result(1, 0) == 0);
            REQUIRE(result(2, 0) == 2);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) += Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) += Matr(6, 3), MatrixError);
        }
    }

    SECTION("operator+(Matrix, double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            auto result = m1 + 6;

            REQUIRE(result(0, 0) == 7);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            auto result = m1 + 6;

            REQUIRE(result(0, 0) == 7);
            REQUIRE(result(0, 1) == 8);
            REQUIRE(result(1, 0) == 9);
            REQUIRE(result(1, 1) == 10);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            auto result = m1 + 6;

            REQUIRE(result(0, 0) == 7);
            REQUIRE(result(0, 1) == 8);
            REQUIRE(result(0, 2) == 9);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            auto result = m1 + 6;

            REQUIRE(result(0, 0) == 7);
            REQUIRE(result(1, 0) == 8);
            REQUIRE(result(2, 0) == 9);
        }
    }
}

TEST_CASE("operator-", "[matrix]")
{
    SECTION("operator-(Matrix, Matrix)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};
            Matr m2{1, 1, -5.0};

            auto result = m1 - m2;

            REQUIRE(result(0, 0) == 6);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            Matr m2{2, 2, 0.0}; // [4 3; 2 1]
            m2(0, 0) = 4;
            m2(0, 1) = 3;
            m2(1, 0) = 2;
            m2(1, 1) = 1;

            auto result = m1 - m2;

            REQUIRE(result(0, 0) == -3);
            REQUIRE(result(0, 1) == -1);
            REQUIRE(result(1, 0) == 1);
            REQUIRE(result(1, 1) == 3);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            Matr m2{1, 3}; // [-3 -2 -1]
            m2(0, 0) = -3;
            m2(0, 1) = -2;
            m2(0, 2) = -1;

            auto result = m1 - m2;

            REQUIRE(result(0, 0) == 4);
            REQUIRE(result(0, 1) == 4);
            REQUIRE(result(0, 2) == 4);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            Matr m2{3, 1}; // [-3;-2;-1]
            m2(0, 0) = -3;
            m2(1, 0) = -2;
            m2(2, 0) = -1;

            auto result = m1 - m2;

            REQUIRE(result(0, 0) == 4);
            REQUIRE(result(1, 0) == 4);
            REQUIRE(result(2, 0) == 4);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) -= Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) -= Matr(6, 3), MatrixError);
        }
    }

    SECTION("operator-(Matrix, double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            auto result = m1 - 6;

            REQUIRE(result(0, 0) == -5);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            auto result = m1 - 6;

            REQUIRE(result(0, 0) == -5);
            REQUIRE(result(0, 1) == -4);
            REQUIRE(result(1, 0) == -3);
            REQUIRE(result(1, 1) == -2);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            auto result = m1 - 6;

            REQUIRE(result(0, 0) == -5);
            REQUIRE(result(0, 1) == -4);
            REQUIRE(result(0, 2) == -3);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            auto result = m1 - 6;

            REQUIRE(result(0, 0) == -5);
            REQUIRE(result(1, 0) == -4);
            REQUIRE(result(2, 0) == -3);
        }
    }
}

TEST_CASE("operator*", "[matrix]")
{
    SECTION("operator*(Matrix, Matrix)")
    {
        SECTION("[1x1] LHS")
        {
            Matr m1{1, 1, 1.0};

            SECTION("[1x1] RHS")
            {
                Matr m2{1, 1, -5.0};

                auto result = m1 * m2;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == -5);
            }

            SECTION("[1x4] RHS")
            {
                Matr m2{1, 4, 0.0};
                m2(0, 0) = 1;
                m2(0, 0) = 2;
                m2(0, 0) = 3;
                m2(0, 0) = 4;

                auto result = m1 * m2;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 4);
                result(0, 0) = 1;
                result(0, 1) = 2;
                result(0, 2) = 3;
                result(0, 3) = 4;
            }
        }

        SECTION("[2x2] LHS")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            SECTION("[2x2] RHS")
            {
                Matr m2{2, 2, 0.0}; // [4 3; 2 1]
                m2(0, 0) = 4;
                m2(0, 1) = 3;
                m2(1, 0) = 2;
                m2(1, 1) = 1;

                auto result = m1 * m2;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 8);
                REQUIRE(result(0, 1) == 5);
                REQUIRE(result(1, 0) == 20);
                REQUIRE(result(1, 1) == 13);
            }

            SECTION("[2x1] RHS")
            {
                Matr m2{2, 1, 0.0}; // [-1; -2]
                m2(0, 0) = -1;
                m2(1, 0) = -2;

                auto result = m1 * m2;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == -5);
                REQUIRE(result(1, 0) == -11);
            }
        }

        SECTION("[1x3] LHS")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            SECTION("[3x1] RHS")
            {
                Matr m2{3, 1}; // [3;2;1]
                m2(0, 0) = 3;
                m2(1, 0) = 2;
                m2(2, 0) = 1;

                auto result = m1 * m2;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 10);
            }
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(Matr(2, 2) *= Matr(1, 2), MatrixError);
            REQUIRE_THROWS_AS(Matr(2, 1) *= Matr(2, 1), MatrixError);
        }
    }

    SECTION("operator*(Matrix, double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            auto result = m1 * 6;

            REQUIRE(result(0, 0) == 6);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            auto result = m1 * 6;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(0, 1) == 12);
            REQUIRE(result(1, 0) == 18);
            REQUIRE(result(1, 1) == 24);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            auto result = m1 * 6;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(0, 1) == 12);
            REQUIRE(result(0, 2) == 18);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            auto result = m1 * 6;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(1, 0) == 12);
            REQUIRE(result(2, 0) == 18);
        }
    }

    SECTION("operator*(double, Matrix)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            auto result = 6 * m1;

            REQUIRE(result(0, 0) == 6);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(1, 0) = 3;
            m1(1, 1) = 4;

            auto result = 6 * m1;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(0, 1) == 12);
            REQUIRE(result(1, 0) == 18);
            REQUIRE(result(1, 1) == 24);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 1;
            m1(0, 1) = 2;
            m1(0, 2) = 3;

            auto result = 6 * m1;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(0, 1) == 12);
            REQUIRE(result(0, 2) == 18);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 1;
            m1(1, 0) = 2;
            m1(2, 0) = 3;

            auto result = 6 * m1;

            REQUIRE(result(0, 0) == 6);
            REQUIRE(result(1, 0) == 12);
            REQUIRE(result(2, 0) == 18);
        }
    }
}

TEST_CASE("operator/", "[matrix]")
{
    SECTION("operator/=(double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 6};

            auto result = m1 / 6;

            REQUIRE(result(0, 0) == 1);
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = -24;
            m1(0, 1) = -18;
            m1(1, 0) = -12;
            m1(1, 1) = -3;

            auto result = m1 / 6;

            REQUIRE(result(0, 0) == -4);
            REQUIRE(result(0, 1) == -3);
            REQUIRE(result(1, 0) == -2);
            REQUIRE(result(1, 1) == -0.5);
        }

        SECTION("1x3 vector")
        {
            Matr m1{1, 3}; // [1 2 3]
            m1(0, 0) = 3;
            m1(0, 1) = 9;
            m1(0, 2) = 18;

            auto result = m1 / 6;

            REQUIRE(result(0, 0) == 0.5);
            REQUIRE(result(0, 1) == 1.5);
            REQUIRE(result(0, 2) == 3);
        }

        SECTION("3x1 vector")
        {
            Matr m1{3, 1}; // [1;2;3]
            m1(0, 0) = 3;
            m1(1, 0) = 9;
            m1(2, 0) = 18;

            auto result = m1 / 6;

            REQUIRE(result(0, 0) == 0.5);
            REQUIRE(result(1, 0) == 1.5);
            REQUIRE(result(2, 0) == 3);
        }

        SECTION("invalid - division by zero")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) / 0, MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) / 0, MatrixError);
        }
    }
}

TEST_CASE("operator<", "[matrix]")
{
    SECTION("operator<(Matrix, double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            SECTION("less than 0")
            {
                auto result = m1 < 0;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 0);
            }

            SECTION("less than 1")
            {
                auto result = m1 < 1;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 0);
            }

            SECTION("less than 2")
            {
                auto result = m1 < 2;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 1);
            }
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [1 2; 3 4]
            m1(0, 0) = 0.5;
            m1(0, 1) = 1;
            m1(1, 0) = 1.5;
            m1(1, 1) = 2;

            SECTION("less than 1")
            {
                auto result = m1 < 1;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 1);
                REQUIRE(result(0, 1) == 0);
                REQUIRE(result(1, 0) == 0);
                REQUIRE(result(1, 1) == 0);
            }

            SECTION("less than 1.5")
            {
                auto result = m1 < 1.5;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 1);
                REQUIRE(result(0, 1) == 1);
                REQUIRE(result(1, 0) == 0);
                REQUIRE(result(1, 1) == 0);
            }

            SECTION("less than 2")
            {
                auto result = m1 < 2;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 1);
                REQUIRE(result(0, 1) == 1);
                REQUIRE(result(1, 0) == 1);
                REQUIRE(result(1, 1) == 0);
            }
        }
    }
}

TEST_CASE("operator>", "[matrix]")
{
    SECTION("operator>(Matrix, double)")
    {
        SECTION("1x1 matrix")
        {
            Matr m1{1, 1, 1.0};

            SECTION("greater than 0")
            {
                auto result = m1 > 0;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 1);
            }

            SECTION("greater than 1")
            {
                auto result = m1 > 1;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 0);
            }

            SECTION("greater than 2")
            {
                auto result = m1 > 2;

                REQUIRE(result.n_rows() == 1);
                REQUIRE(result.n_cols() == 1);
                REQUIRE(result(0, 0) == 0);
            }
        }

        SECTION("2x2 matrix")
        {
            Matr m1{2, 2, 0.0}; // [0.5 2; 1.5 2]
            m1(0, 0) = 0.5;
            m1(0, 1) = 1;
            m1(1, 0) = 1.5;
            m1(1, 1) = 2;

            SECTION("greater than 1")
            {
                auto result = m1 > 1;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 0);
                REQUIRE(result(0, 1) == 0);
                REQUIRE(result(1, 0) == 1);
                REQUIRE(result(1, 1) == 1);
            }

            SECTION("greater than 1.5")
            {
                auto result = m1 > 1.5;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 0);
                REQUIRE(result(0, 1) == 0);
                REQUIRE(result(1, 0) == 0);
                REQUIRE(result(1, 1) == 1);
            }

            SECTION("greater than 2")
            {
                auto result = m1 > 2;

                REQUIRE(result.n_rows() == 2);
                REQUIRE(result.n_cols() == 2);
                REQUIRE(result(0, 0) == 0);
                REQUIRE(result(0, 1) == 0);
                REQUIRE(result(1, 0) == 0);
                REQUIRE(result(1, 1) == 0);
            }
        }
    }
}