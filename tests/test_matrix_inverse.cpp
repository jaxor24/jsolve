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
    }
}