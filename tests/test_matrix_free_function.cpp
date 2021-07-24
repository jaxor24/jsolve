#include "test_includes.h"

#include "matrix.h"

using Matr = Matrix<double>;

TEST_CASE("mult_elem", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m1{ 1, 1, 6 };
        Matr m2{ 1, 1, -2.5 };

        auto result = mult_elem(m1, m2);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);
        REQUIRE(result(0, 0) == 6 * -2.5);
    }

    SECTION("2x2 matrix")
    {
        Matr m1{ 2, 2, 0.0 };
        m1(0, 0) = -1;
        m1(0, 1) = -2;
        m1(1, 0) = -3;
        m1(1, 1) = -4;

        Matr m2{ 2, 2, 0.0 };
        m2(0, 0) = 4;
        m2(0, 1) = 3;
        m2(1, 0) = 2;
        m2(1, 1) = 1;

        auto result = mult_elem(m1, m2);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 2);
        REQUIRE(result(0, 0) == -1 * 4);
        REQUIRE(result(0, 1) == -2 * 3);
        REQUIRE(result(1, 0) == -3 * 2);
        REQUIRE(result(1, 1) == -4 * 1);
    }

    SECTION("1x3 vector")
    {
        Matr m1{ 1, 3 };
        m1(0, 0) = 3;
        m1(0, 1) = 9;
        m1(0, 2) = 18;

        Matr m2{ 1, 3 };
        m2(0, 0) = -0.5;
        m2(0, 1) = 0.5;
        m2(0, 2) = 0.0;

        auto result = mult_elem(m1, m2);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 3);
        REQUIRE(result(0, 0) == 3 * -0.5);
        REQUIRE(result(0, 1) == 9 * 0.5);
        REQUIRE(result(0, 2) == 18 * 0.0);
    }

    SECTION("invalid dimensions")
    {
        REQUIRE_THROWS_AS(mult_elem(Matr(2, 2), Matr(1, 2)), MatrixError);
        REQUIRE_THROWS_AS(mult_elem(Matr(2, 1), Matr(2, 3)), MatrixError);
    }
}

TEST_CASE("div_elem", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m1{ 1, 1, 6 };
        Matr m2{ 1, 1, -2.5 };

        auto result = div_elem(m1, m2);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 1);
        REQUIRE(result(0, 0) == 6 / -2.5);
    }

    SECTION("2x2 matrix")
    {
        Matr m1{ 2, 2, 0.0 };
        m1(0, 0) = -1;
        m1(0, 1) = -2;
        m1(1, 0) = -3;
        m1(1, 1) = -4;

        Matr m2{ 2, 2, 0.0 };
        m2(0, 0) = 4;
        m2(0, 1) = 3;
        m2(1, 0) = 2;
        m2(1, 1) = 1;

        auto result = div_elem(m1, m2);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 2);
        REQUIRE(result(0, 0) == -1.0 / 4);
        REQUIRE(result(0, 1) == -2.0 / 3);
        REQUIRE(result(1, 0) == -3.0 / 2);
        REQUIRE(result(1, 1) == -4.0 / 1);
    }

    SECTION("1x3 vector")
    {
        Matr m1{ 1, 3 };
        m1(0, 0) = 3;
        m1(0, 1) = 9;
        m1(0, 2) = 18;

        Matr m2{ 1, 3 };
        m2(0, 0) = -0.5;
        m2(0, 1) = 0.5;
        m2(0, 2) = 10;

        auto result = div_elem(m1, m2);

        REQUIRE(result.n_rows() == 1);
        REQUIRE(result.n_cols() == 3);
        REQUIRE(result(0, 0) == 3.0 / -0.5);
        REQUIRE(result(0, 1) == 9.0 / 0.5);
        REQUIRE(result(0, 2) == 18.0 / 10);
    }

    SECTION("division by zero")
    {
        auto m1 = Matr(2, 2, 1.0);
        auto m2 = Matr(2, 2, 0.0);

        auto result = div_elem(m1, m2);

        REQUIRE(result.n_rows() == 2);
        REQUIRE(result.n_cols() == 2);
        REQUIRE(result(0, 0) == std::numeric_limits<Matr::value_type>::infinity());
        REQUIRE(result(0, 1) == std::numeric_limits<Matr::value_type>::infinity());
        REQUIRE(result(1, 0) == std::numeric_limits<Matr::value_type>::infinity());
        REQUIRE(result(1, 1) == std::numeric_limits<Matr::value_type>::infinity());
    }

    SECTION("invalid dimensions")
    {
        REQUIRE_THROWS_AS(div_elem(Matr(2, 2), Matr(1, 2)), MatrixError);
        REQUIRE_THROWS_AS(div_elem(Matr(2, 1), Matr(2, 3)), MatrixError);
    }
}
