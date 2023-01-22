#include "test_includes.h"

#include "matrix.h"

#include <stdexcept>

using Matr = Matrix<double>;

TEST_CASE("matrix::matrix(row,col)", "[matrix]")
{
    SECTION("construction - invalid")
    {
        REQUIRE_THROWS_AS(Matr(0, 0), MatrixError);
    }

    SECTION("construction - valid")
    {
        REQUIRE_NOTHROW(Matr{3, 6});
        REQUIRE_NOTHROW(Matr{20, 1});
        REQUIRE_NOTHROW(Matr{1, 500});
        REQUIRE_NOTHROW(Matr{1, 1});
        REQUIRE_NOTHROW(Matr{3, 3});
    }
}

TEST_CASE("matrix::matrix(row,col,val)", "[matrix]")
{
    SECTION("construction - valid")
    {
        REQUIRE_NOTHROW(Matr{3, 6, 10.0});
        REQUIRE_NOTHROW(Matr{1, 1, -500});
        REQUIRE_NOTHROW(Matr{100, 100, -500});
    }

    SECTION("construction - invalid")
    {
        REQUIRE_THROWS_AS(Matr(0, 0, 10.0), MatrixError);
    }
}

TEST_CASE("matrix::n_rows", "[matrix]")
{
    SECTION("non-zero rows")
    {
        unsigned n_rows{10};
        unsigned n_cols{5};

        auto m = Matr{n_rows, n_cols};

        REQUIRE(m.n_rows() == n_rows);
    }

    SECTION("zero rows")
    {
        unsigned n_rows{1};
        unsigned n_cols{5};

        auto m = Matr{n_rows, n_cols};

        REQUIRE(m.n_rows() == n_rows);
    }
}

TEST_CASE("matrix::n_cols", "[matrix]")
{
    SECTION("non-zero rows")
    {
        unsigned n_rows{10};
        unsigned n_cols{5};

        auto m = Matr{n_rows, n_cols};

        REQUIRE(m.n_cols() == n_cols);
    }

    SECTION("zero rows")
    {
        unsigned n_rows{1};
        unsigned n_cols{5};

        auto m = Matr{n_rows, n_cols};

        REQUIRE(m.n_cols() == n_cols);
    }
}

TEST_CASE("matrix::make_transpose", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{1, 1, 1.0};
        auto transpose = m.make_transpose();
        REQUIRE(transpose.n_rows() == m.n_cols());
        REQUIRE(transpose.n_cols() == m.n_rows());
        REQUIRE(transpose(0, 0) == 1);
    }

    SECTION("2x2 matrix")
    {
        Matr m{2, 2, 0.0};
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 0) = 3;
        m(1, 1) = 4;
        auto transpose = m.make_transpose();
        REQUIRE(transpose.n_rows() == m.n_cols());
        REQUIRE(transpose.n_cols() == m.n_rows());
        REQUIRE(transpose(0, 0) == 1);
        REQUIRE(transpose(1, 1) == 4);
        REQUIRE(transpose(0, 1) == 3);
        REQUIRE(transpose(1, 0) == 2);
    }

    SECTION("1x3 vector")
    {
        Matr m{1, 3};
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        auto transpose = m.make_transpose();
        REQUIRE(transpose.n_rows() == m.n_cols());
        REQUIRE(transpose.n_cols() == m.n_rows());
        REQUIRE(transpose(0, 0) == 1);
        REQUIRE(transpose(1, 0) == 2);
        REQUIRE(transpose(2, 0) == 3);
    }

    SECTION("3x1 vector")
    {
        Matr m{3, 1};
        m(0, 0) = 1;
        m(1, 0) = 2;
        m(2, 0) = 3;
        auto transpose = m.make_transpose();
        REQUIRE(transpose.n_rows() == m.n_cols());
        REQUIRE(transpose.n_cols() == m.n_rows());
        REQUIRE(transpose(0, 0) == 1);
        REQUIRE(transpose(0, 1) == 2);
        REQUIRE(transpose(0, 2) == 3);
    }
}

TEST_CASE("matrix::slice", "[matrix]")
{
    SECTION("3x3 matrix")
    {
        Matr m{3, 3, 0.0};
        m(0, 0) = 1;
        m(1, 0) = 4;
        m(2, 0) = 7;

        m(0, 1) = 2;
        m(1, 1) = 5;
        m(2, 1) = 8;

        m(0, 2) = 3;
        m(1, 2) = 6;
        m(2, 2) = 9;

#ifndef NDEBUG
        SECTION("slice outside bounds")
        {
            // Bounds checking only done in debug builds for performance reasons
            REQUIRE_THROWS_AS(m.slice(m.n_rows(), {}), std::out_of_range);
            REQUIRE_THROWS_AS(m.slice({}, m.n_cols()), std::out_of_range);
        }
#endif

        SECTION("slice all")
        {
            auto slice = m.slice({}, {});
            REQUIRE(slice == m);
        }

        SECTION("slice first column")
        {
            auto slice = m.slice({}, {0, 0});
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 1);
            REQUIRE(slice(1, 0) == 4);
            REQUIRE(slice(2, 0) == 7);
        }

        SECTION("slice first row")
        {
            auto slice = m.slice({0, 0}, {});
            REQUIRE(slice.n_rows() == 1);
            REQUIRE(slice.n_cols() == m.n_rows());
            REQUIRE(slice(0, 0) == 1);
            REQUIRE(slice(0, 1) == 2);
            REQUIRE(slice(0, 2) == 3);
        }

        SECTION("slice last column")
        {
            auto slice = m.slice({}, {2, 2});
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 3);
            REQUIRE(slice(1, 0) == 6);
            REQUIRE(slice(2, 0) == 9);
        }

        SECTION("slice last row")
        {
            auto slice = m.slice({2, 2}, {});
            REQUIRE(slice.n_rows() == 1);
            REQUIRE(slice.n_cols() == m.n_rows());
            REQUIRE(slice(0, 0) == 7);
            REQUIRE(slice(0, 1) == 8);
            REQUIRE(slice(0, 2) == 9);
        }
    }

    SECTION("1x3 vector")
    {
        Matr m{1, 3};
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;

#ifndef NDEBUG
        SECTION("slice outside bounds")
        {
            // Bounds checking only done in debug builds for performance reasons
            REQUIRE_THROWS_AS(m.slice(m.n_rows(), {}), std::out_of_range);
            REQUIRE_THROWS_AS(m.slice({}, m.n_cols()), std::out_of_range);
        }
#endif

        SECTION("slice all")
        {
            auto slice = m.slice({}, {});
            REQUIRE(slice == m);
        }

        SECTION("slice first column")
        {
            auto slice = m.slice({}, {0, 0});
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 1);
        }

        SECTION("slice first row")
        {
            auto slice = m.slice({0, 0}, {});
            REQUIRE(slice.n_rows() == 1);
            REQUIRE(slice.n_cols() == m.n_cols());
            REQUIRE(slice(0, 0) == 1);
            REQUIRE(slice(0, 1) == 2);
            REQUIRE(slice(0, 2) == 3);
        }

        SECTION("slice last column")
        {
            auto slice = m.slice({}, {2, 2});
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 3);
        }
    }
}

TEST_CASE("matrix::update", "[matrix]")
{
    SECTION("update entire matrix")
    {
        Matr m1{2, 2, 0};

        Matr sub{2, 2, 0};

        sub(0, 0) = 1;
        sub(0, 1) = 2;
        sub(1, 0) = 3;
        sub(1, 1) = 4;

        m1.update({}, {}, sub);

        REQUIRE(m1 == sub);
    }

    SECTION("update 4x4 matrix")
    {
        Matr m1{4, 4, 0};

        SECTION("with 2x2 matrix")
        {
            Matr sub{2, 2, 0};
            sub(0, 0) = 1;
            sub(0, 1) = 2;
            sub(1, 0) = 3;
            sub(1, 1) = 4;

            SECTION("in the top left")
            {
                m1.update({0, 1}, {0, 1}, sub);

                REQUIRE(m1.n_cols() == 4);
                REQUIRE(m1.n_rows() == 4);
                REQUIRE(m1(0, 0) == 1);
                REQUIRE(m1(0, 1) == 2);
                REQUIRE(m1(0, 2) == 0);
                REQUIRE(m1(0, 3) == 0);
                REQUIRE(m1(1, 0) == 3);
                REQUIRE(m1(1, 1) == 4);
                REQUIRE(m1(1, 2) == 0);
                REQUIRE(m1(1, 3) == 0);
                REQUIRE(m1(2, 0) == 0);
                REQUIRE(m1(2, 1) == 0);
                REQUIRE(m1(2, 2) == 0);
                REQUIRE(m1(2, 3) == 0);
                REQUIRE(m1(3, 0) == 0);
                REQUIRE(m1(3, 1) == 0);
                REQUIRE(m1(3, 2) == 0);
                REQUIRE(m1(3, 3) == 0);
            }

            SECTION("in the top right")
            {
                m1.update({0, 1}, {2, 3}, sub);

                REQUIRE(m1.n_cols() == 4);
                REQUIRE(m1.n_rows() == 4);
                REQUIRE(m1(0, 0) == 0);
                REQUIRE(m1(0, 1) == 0);
                REQUIRE(m1(0, 2) == 1);
                REQUIRE(m1(0, 3) == 2);
                REQUIRE(m1(1, 0) == 0);
                REQUIRE(m1(1, 1) == 0);
                REQUIRE(m1(1, 2) == 3);
                REQUIRE(m1(1, 3) == 4);
                REQUIRE(m1(2, 0) == 0);
                REQUIRE(m1(2, 1) == 0);
                REQUIRE(m1(2, 2) == 0);
                REQUIRE(m1(2, 3) == 0);
                REQUIRE(m1(3, 0) == 0);
                REQUIRE(m1(3, 1) == 0);
                REQUIRE(m1(3, 2) == 0);
                REQUIRE(m1(3, 3) == 0);
            }
        }
    }

    SECTION("invalid inputs")
    {
        Matr m1{10, 10, 0};

        SECTION("more range rows than sub matrix")
        {
            Matr sub{1, 1, 0};
            REQUIRE_THROWS_AS(m1.update({0, 1}, {1, 1}, sub), MatrixError);
        }

        SECTION("more range cols than sub matrix")
        {
            Matr sub{1, 1, 0};
            REQUIRE_THROWS_AS(m1.update({1, 1}, {0, 1}, sub), MatrixError);
        }

        SECTION("less range rows than sub matrix")
        {
            Matr sub{2, 2, 0};
            REQUIRE_THROWS_AS(m1.update({0, 0}, {0, 1}, sub), MatrixError);
        }

        SECTION("less range cols than sub matrix")
        {
            Matr sub{2, 2, 0};
            REQUIRE_THROWS_AS(m1.update({0, 1}, {0, 0}, sub), MatrixError);
        }

        SECTION("sub matrix more rows than target matrix")
        {
            Matr sub{11, 10, 0};
            REQUIRE_THROWS_AS(m1.update({}, {}, sub), MatrixError);
        }

        SECTION("sub matrix more cols than target matrix")
        {
            Matr sub{10, 11, 0};
            REQUIRE_THROWS_AS(m1.update({}, {}, sub), MatrixError);
        }

        SECTION("sub matrix less rows than target matrix")
        {
            Matr sub{2, 10, 0};
            REQUIRE_THROWS_AS(m1.update({}, {}, sub), MatrixError);
        }

        SECTION("sub matrix less cols than target matrix")
        {
            Matr sub{10, 2, 0};
            REQUIRE_THROWS_AS(m1.update({}, {}, sub), MatrixError);
        }
    }
}

TEST_CASE("matrix::begin/end", "[matrix]")
{
    SECTION("begin/end")
    {
        SECTION("2x2 matrix")
        {
            Matr m{2, 2, 0.0}; // [1 2; 3 4]
            m(0, 0) = 1;
            m(0, 1) = 2;
            m(1, 0) = 3;
            m(1, 1) = 4;

            auto begin = m.begin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(*begin == 3);
            ++begin;
            REQUIRE(*begin == 4);
            ++begin;
            REQUIRE(begin == m.end());
        }

        SECTION("2x2 matrix")
        {
            Matr m{2, 2, 0.0};
            // [0 0; 0 0] -> [1 2; 3 4]

            auto begin = m.begin();

            *begin = 1;
            REQUIRE(*begin == 1);
            ++begin;

            *begin = 2;
            REQUIRE(*begin == 2);
            ++begin;

            *begin = 3;
            REQUIRE(*begin == 3);
            ++begin;

            *begin = 4;
            REQUIRE(*begin == 4);
            ++begin;

            REQUIRE(begin == m.end());
        }

        SECTION("1x2 matrix")
        {
            Matr m{1, 2, 0.0}; // [1 2]
            m(0, 0) = 1;
            m(0, 1) = 2;

            auto begin = m.begin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(begin == m.end());
        }

        SECTION("2x1 matrix")
        {
            Matr m{2, 1, 0.0}; // [1 ; 2]
            m(0, 0) = 1;
            m(1, 0) = 2;

            auto begin = m.begin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(begin == m.end());
        }
    }

    SECTION("begin/end const")
    {
        SECTION("2x2 matrix")
        {
            const Matr m{2, 2, 0.0};

            auto begin = m.begin();

            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(begin == m.end());
        }

        SECTION("1x2 matrix")
        {
            const Matr m{1, 2, 0.0};

            auto begin = m.begin();

            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(begin == m.end());
        }

        SECTION("2x1 matrix")
        {
            const Matr m{2, 1, 0.0};

            auto begin = m.begin();

            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(*begin == 0);
            ++begin;
            REQUIRE(begin == m.end());
        }
    }

    SECTION("cbegin/cend")
    {
        SECTION("2x2 matrix")
        {
            Matr m{2, 2, 0.0}; // [1 2; 3 4]
            m(0, 0) = 1;
            m(0, 1) = 2;
            m(1, 0) = 3;
            m(1, 1) = 4;

            auto begin = m.cbegin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(*begin == 3);
            ++begin;
            REQUIRE(*begin == 4);
            ++begin;
            REQUIRE(begin == m.cend());
        }

        SECTION("1x2 matrix")
        {
            Matr m{1, 2, 0.0}; // [1 2]
            m(0, 0) = 1;
            m(0, 1) = 2;

            auto begin = m.cbegin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(begin == m.cend());
        }

        SECTION("2x1 matrix")
        {
            Matr m{2, 1, 0.0}; // [1 ; 2]
            m(0, 0) = 1;
            m(1, 0) = 2;

            auto begin = m.cbegin();

            REQUIRE(*begin == 1);
            ++begin;
            REQUIRE(*begin == 2);
            ++begin;
            REQUIRE(begin == m.cend());
        }
    }
}
