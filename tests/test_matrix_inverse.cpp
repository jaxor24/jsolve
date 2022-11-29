#include "test_includes.h"

#include "matrix.h"

using Matr = Matrix<double>;

TEST_CASE("swap_rows", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{1, 1, 6};
        // [6]

        swap_rows(m, 0, 0);

        REQUIRE(m.n_rows() == 1);
        REQUIRE(m.n_cols() == 1);
        REQUIRE(m(0, 0) == 6);
    }

    SECTION("2x2 matrix")
    {
        Matr m{2, 2, 0.0};
        m(0, 0) = -1;
        m(0, 1) = -2;
        m(1, 0) = -3;
        m(1, 1) = -4;
        // [-1 -2; -3 -4]

        SECTION("row 1 and 2")
        {
            swap_rows(m, 0, 1);

            REQUIRE(m(0, 0) == -3);
            REQUIRE(m(0, 1) == -4);
            REQUIRE(m(1, 0) == -1);
            REQUIRE(m(1, 1) == -2);
        }

        SECTION("row 2 and 1")
        {
            swap_rows(m, 1, 0);

            REQUIRE(m(0, 0) == -3);
            REQUIRE(m(0, 1) == -4);
            REQUIRE(m(1, 0) == -1);
            REQUIRE(m(1, 1) == -2);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(swap_rows(m, 2, 2), MatrixError);
        }
    }

    SECTION("1x2 matrix")
    {
        Matr m{1, 2, 0.0};
        m(0, 0) = 1;
        m(0, 1) = 2;
        // [1 2]

        swap_rows(m, 0, 0);

        REQUIRE(m(0, 0) == 1);
        REQUIRE(m(0, 1) == 2);
    }

    SECTION("2x1 matrix")
    {
        Matr m{2, 1, 0.0};
        m(0, 0) = 1;
        m(1, 0) = 2;
        // [1 ; 2]

        SECTION("row 1 and 2")
        {
            swap_rows(m, 0, 1);

            REQUIRE(m(0, 0) == 2);
            REQUIRE(m(0, 1) == 1);
        }

        SECTION("row 2 and 1")
        {
            swap_rows(m, 1, 0);

            REQUIRE(m(0, 0) == 2);
            REQUIRE(m(0, 1) == 1);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(swap_rows(m, 3, 4), MatrixError);
        }
    }

    SECTION("3x3 matrix")
    {
        Matr m{3, 3, 0.0};
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;

        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        m(2, 0) = 7;
        m(2, 1) = 8;
        m(2, 2) = 9;

        // [1,2,3; 4,5,6; 7,8,9]

        SECTION("row 1 and 3")
        {
            swap_rows(m, 0, 2);

            REQUIRE(m(0, 0) == 7);
            REQUIRE(m(0, 1) == 8);
            REQUIRE(m(0, 2) == 9);

            REQUIRE(m(1, 0) == 4);
            REQUIRE(m(1, 1) == 5);
            REQUIRE(m(1, 2) == 6);

            REQUIRE(m(2, 0) == 1);
            REQUIRE(m(2, 1) == 2);
            REQUIRE(m(2, 2) == 3);
        }

        SECTION("invalid dimensions")
        {
            REQUIRE_THROWS_AS(swap_rows(m, 4, 4), MatrixError);
        }
    }
}

TEST_CASE("inverse", "[matrix]")
{
    SECTION("random matrix")
    {
        // Generate random matrix
        auto n = std::size_t{10};
        auto elements = GENERATE(take(100, chunk(10*10, random(-1000.0, 1000.0))));

        Matr m{n, n, 0.0};

        for (std::size_t i{ 0 } ; auto& e : m)
        {
            e = elements[i];
            i++;
        }

        // Invert
        auto identity = m * inverse(m);

        INFO("Matrix: " << identity);

        // Assert it is the identity matrix
        for (std::size_t i{0}; auto& e : identity)
        {
            auto d = std::lldiv(i, n);
            if (d.quot == d.rem)
            {
                // Diagonal
                CHECK_THAT(e, Catch::Matchers::WithinAbs(1, 1e-8));
            }
            else
            {
                // Off diagonal
                CHECK_THAT(e, Catch::Matchers::WithinAbs(0, 1e-8));
            }
            i++;
        }
    }

    SECTION("1x1 matrix")
    {
        Matr m{1, 1, 6};
        // [6]

        auto result = inverse(m);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);
        REQUIRE(result(0, 0) == 1.0 / 6);
    }

    SECTION("2x2 matrix")
    {
        Matr m{2, 2, 0.0};
        m(0, 0) = -1;
        m(0, 1) = 3.0 / 2;
        m(1, 0) = 1;
        m(1, 1) = -1;

        auto result = inverse(m);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 2);
        REQUIRE(result(0, 0) == 2);
        REQUIRE(result(0, 1) == 3);
        REQUIRE(result(1, 0) == 2);
        REQUIRE(result(1, 1) == 2);
    }

    SECTION("3x3 matrix")
    {
        SECTION("A")
        {
            Matr m{3, 3, 0.0};
            m(0, 0) = 2;
            m(0, 1) = -1;
            m(0, 2) = 0;

            m(1, 0) = -1;
            m(1, 1) = 2;
            m(1, 2) = -1;

            m(2, 0) = 0;
            m(2, 1) = -1;
            m(2, 2) = 2;

            auto result = inverse(m);

            REQUIRE(result.n_rows() == 3);
            REQUIRE(result.n_cols() == 3);

            REQUIRE(approx_equal(result(0, 0), 0.75));
            REQUIRE(approx_equal(result(0, 1), 0.5));
            REQUIRE(approx_equal(result(0, 2), 0.25));

            REQUIRE(approx_equal(result(1, 0), 0.5));
            REQUIRE(approx_equal(result(1, 1), 1));
            REQUIRE(approx_equal(result(1, 2), 0.5));

            REQUIRE(approx_equal(result(2, 0), 0.25));
            REQUIRE(approx_equal(result(2, 1), 0.5));
            REQUIRE(approx_equal(result(2, 2), 0.75));
        }

        SECTION("B")
        {
            Matr m{3, 3, 0.0};
            m(0, 0) = 1;
            m(0, 1) = 1;
            m(0, 2) = 3;

            m(1, 0) = 1;
            m(1, 1) = 3;
            m(1, 2) = -3;

            m(2, 0) = -2;
            m(2, 1) = -4;
            m(2, 2) = -4;

            auto result = inverse(m);

            REQUIRE(result.n_rows() == 3);
            REQUIRE(result.n_cols() == 3);

            REQUIRE(approx_equal(result(0, 0), 3));
            REQUIRE(approx_equal(result(0, 1), 1));
            REQUIRE(approx_equal(result(0, 2), 1.5));

            REQUIRE(approx_equal(result(1, 0), -1.25));
            REQUIRE(approx_equal(result(1, 1), -0.25));
            REQUIRE(approx_equal(result(1, 2), -0.75));

            REQUIRE(approx_equal(result(2, 0), -0.25));
            REQUIRE(approx_equal(result(2, 1), -0.25));
            REQUIRE(approx_equal(result(2, 2), -0.25));
        }
    }
}