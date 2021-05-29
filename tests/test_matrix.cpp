#include "test_includes.h"

#include "matrix.h"

using Matr = Matrix<double>;

TEST_CASE("matrix::matrix(row,col)", "[matrix]")
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

TEST_CASE("matrix::matrix(row,col,val)", "[matrix]")
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

TEST_CASE("matrix::n_rows, matrix::n_cols", "[matrix]")
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

TEST_CASE("matrix::min, matrix::max", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{ 1, 1, 5.0 };
        REQUIRE(m.max() == 5.0);
        REQUIRE(m.min() == 5.0);
    }
        
    SECTION("1x3 vector")
    {
        Matr m{ 1, 3 };
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;

        REQUIRE(m.max() == 3.0);
        REQUIRE(m.min() == 1.0);

    }

    SECTION("3x1 vector")
    {
        Matr m{ 3, 1 };
        m(0, 0) = 1;
        m(1, 0) = 2;
        m(2, 0) = 3;

        REQUIRE(m.max() == 3.0);
        REQUIRE(m.min() == 1.0);
    }

    SECTION("3x3 matrix")
    {
        Matr m{ 3, 3, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 400;
        m(2, 0) = 7;

        m(0, 1) = 2;
        m(1, 1) = 5;
        m(2, 1) = 8;

        m(0, 2) = 3;
        m(1, 2) = 6;
        m(2, 2) = -9;

        REQUIRE(m.max() == 400.0);
        REQUIRE(m.min() == -9.0);
    }
}

TEST_CASE("matrix::sum", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{ 1, 1, 5.0 };
        REQUIRE(m.sum() == 5.0);
    }

    SECTION("1x3 vector")
    {
        Matr m{ 1, 3 };
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        REQUIRE(m.sum() == 6.0);
    }

    SECTION("3x1 vector")
    {
        Matr m{ 3, 1 };
        m(0, 0) = 1;
        m(1, 0) = 2;
        m(2, 0) = 3;
        REQUIRE(m.sum() == 6.0);
    }

    SECTION("3x3 matrix")
    {
        Matr m{ 3, 3, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = -50;
        m(2, 0) = 7;

        m(0, 1) = 2;
        m(1, 1) = 5;
        m(2, 1) = 8;

        m(0, 2) = 3;
        m(1, 2) = 6;
        m(2, 2) = -9;

        REQUIRE(m.sum() == -27.0);
    }
}

TEST_CASE("matrix::row_min, matrix::row_max", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{ 1, 1, 5.0 };
        // [5]

        SECTION("row_min")
        {
            auto [result, indices] = m.row_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 5);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0,0) == 0);
        }

        SECTION("row_max")
        {
            auto [result, indices] = m.row_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 5);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
        }
    }

    SECTION("1x3 matrix")
    {
        Matr m{ 1, 3, 0.0 };
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 0;
        // [1,2,0]

        SECTION("row_min")
        {
            auto [result, indices] = m.row_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 0);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 2);
        }

        SECTION("row_max")
        {
            auto [result, indices] = m.row_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 2);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 1);
        }
    }

    SECTION("3x1 matrix")
    {
        Matr m{ 3, 1, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 2;
        m(2, 0) = 0;
        // [1]
        // [2]
        // [0]

        SECTION("row_min")
        {
            auto [result, indices] = m.row_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 3);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(1, 0) == 2);
            REQUIRE(result(2, 0) == 0);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 3);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(1, 0) == 0);
            REQUIRE(indices(2, 0) == 0);
        }

        SECTION("row_min")
        {
            auto [result, indices] = m.row_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 3);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(1, 0) == 2);
            REQUIRE(result(2, 0) == 0);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 3);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(1, 0) == 0);
            REQUIRE(indices(2, 0) == 0);
        }
    }

    SECTION("2x3 matrix")
    {
        Matr m{ 2, 4, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 4;
        m(0, 1) = 9;
        m(1, 1) = 5;
        m(0, 2) = 2;
        m(1, 2) = -5;
        m(0, 3) = 3;
        m(1, 3) = 7;
        // [1,9,2,3]
        // [4,5,-5,7]

        SECTION("row_min")
        {
            auto [result, indices] = m.row_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 2);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(1, 0) == -5);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 2);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(1, 0) == 2);
        }

        SECTION("row_max")
        {
            auto [result, indices] = m.row_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 2);
            REQUIRE(result(0, 0) == 9);
            REQUIRE(result(1, 0) == 7);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 2);
            REQUIRE(indices(0, 0) == 1);
            REQUIRE(indices(1, 0) == 3);
        }
    }
}

TEST_CASE("matrix::col_min, matrix::col_max", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{ 1, 1, 5.0 };
        // [5]

        SECTION("col_min")
        {
            auto [result, indices] = m.col_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 5);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
        }

        SECTION("col_max")
        {
            auto [result, indices] = m.col_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 5);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
        }
    }

    SECTION("1x3 matrix")
    {
        Matr m{ 1, 3, 0.0 };
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 0;
        // [1,2,0]

        SECTION("col_min")
        {
            auto [result, indices] = m.col_min();
            REQUIRE(result.n_cols() == 3);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(0, 1) == 2);
            REQUIRE(result(0, 2) == 0);

            REQUIRE(indices.n_cols() == 3);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(0, 1) == 0);
            REQUIRE(indices(0, 2) == 0);
        }

        SECTION("col_max")
        {
            auto [result, indices] = m.col_max();
            REQUIRE(result.n_cols() == 3);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(0, 1) == 2);
            REQUIRE(result(0, 2) == 0);

            REQUIRE(indices.n_cols() == 3);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(0, 1) == 0);
            REQUIRE(indices(0, 2) == 0);
        }
    }

    SECTION("3x1 matrix")
    {
        Matr m{ 3, 1, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 2;
        m(2, 0) = 0;
        // [1]
        // [2]
        // [0]

        SECTION("col_min")
        {
            auto [result, indices] = m.col_min();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 0);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 2);
        }

        SECTION("col_max")
        {
            auto [result, indices] = m.col_max();
            REQUIRE(result.n_cols() == 1);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 2);

            REQUIRE(indices.n_cols() == 1);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 1);
        }
    }

    SECTION("2x3 matrix")
    {
        Matr m{ 2, 4, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 4;
        m(0, 1) = 9;
        m(1, 1) = 5;
        m(0, 2) = 2;
        m(1, 2) = -5;
        m(0, 3) = 3;
        m(1, 3) = 7;
        // [1,9,2,3]
        // [4,5,-5,7]

        SECTION("col_min")
        {
            auto [result, indices] = m.col_min();
            REQUIRE(result.n_cols() == 4);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 1);
            REQUIRE(result(0, 1) == 5);
            REQUIRE(result(0, 2) == -5);
            REQUIRE(result(0, 3) == 3);

            REQUIRE(indices.n_cols() == 4);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 0);
            REQUIRE(indices(0, 1) == 1);
            REQUIRE(indices(0, 2) == 1);
            REQUIRE(indices(0, 3) == 0);
        }

        SECTION("col_max")
        {
            auto [result, indices] = m.col_max();
            REQUIRE(result.n_cols() == 4);
            REQUIRE(result.n_rows() == 1);
            REQUIRE(result(0, 0) == 4);
            REQUIRE(result(0, 1) == 9);
            REQUIRE(result(0, 2) == 2);
            REQUIRE(result(0, 3) == 7);

            REQUIRE(indices.n_cols() == 4);
            REQUIRE(indices.n_rows() == 1);
            REQUIRE(indices(0, 0) == 1);
            REQUIRE(indices(0, 1) == 0);
            REQUIRE(indices(0, 2) == 0);
            REQUIRE(indices(0, 3) == 1);
        }
    }
}

TEST_CASE("matrix::make_transpose", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m{ 1, 1, 1.0 };
        auto transpose = m.make_transpose();
        REQUIRE(transpose.n_rows() == m.n_cols());
        REQUIRE(transpose.n_cols() == m.n_rows());
        REQUIRE(transpose(0, 0) == 1);
    }

    SECTION("2x2 matrix")
    {
        Matr m{ 2, 2, 0.0 };
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
        Matr m{ 1, 3};
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
        Matr m{ 3, 1};
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
        Matr m{ 3, 3, 0.0 };
        m(0, 0) = 1;
        m(1, 0) = 4;
        m(2, 0) = 7;

        m(0, 1) = 2;
        m(1, 1) = 5;
        m(2, 1) = 8;

        m(0, 2) = 3;
        m(1, 2) = 6;
        m(2, 2) = 9;

        SECTION("slice all")
        {
            auto slice = m.slice({}, {});
            REQUIRE(slice == m);
        }

        SECTION("slice first column")
        {
            auto slice = m.slice({}, { 0, 0 });
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
            auto slice = m.slice({}, { 2, 2 });
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 3);
            REQUIRE(slice(1, 0) == 6);
            REQUIRE(slice(2, 0) == 9);
        }

        SECTION("slice last row")
        {
            auto slice = m.slice({ 2, 2 }, {});
            REQUIRE(slice.n_rows() == 1);
            REQUIRE(slice.n_cols() == m.n_rows());
            REQUIRE(slice(0, 0) == 7);
            REQUIRE(slice(0, 1) == 8);
            REQUIRE(slice(0, 2) == 9);
        }

    }

    SECTION("1x3 vector")
    {
        Matr m{ 1, 3 };
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;

        SECTION("slice all")
        {
            auto slice = m.slice({}, {});
            REQUIRE(slice == m);
        }

        SECTION("slice first column")
        {
            auto slice = m.slice({}, { 0, 0 });
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 1);
        }

        SECTION("slice first row")
        {
            auto slice = m.slice({ 0, 0 }, {});
            REQUIRE(slice.n_rows() == 1);
            REQUIRE(slice.n_cols() == m.n_cols());
            REQUIRE(slice(0, 0) == 1);
            REQUIRE(slice(0, 1) == 2);
            REQUIRE(slice(0, 2) == 3);
        }

        SECTION("slice last column")
        {
            auto slice = m.slice({}, { 2, 2 });
            REQUIRE(slice.n_rows() == m.n_rows());
            REQUIRE(slice.n_cols() == 1);
            REQUIRE(slice(0, 0) == 3);
        }
    }
}

TEST_CASE("matrix::operator()", "[matrix]")
{
    Matr m{ 10, 5 };

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

TEST_CASE("matrix::operator==", "[matrix]")
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

TEST_CASE("matrix::operator!=", "[matrix]")
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

TEST_CASE("matrix::operator++", "[matrix]")
{
    Matr m{ 2, 3 };

    SECTION("prefix")
    {
        int n_times{ 10 };

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

    SECTION("postfix")
    {
        int n_times{ 10 };

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
    Matr m{ 2, 3 };

    SECTION("prefix")
    {
        int n_times{ 10 };

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

    SECTION("postfix")
    {
        int n_times{ 10 };

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
    SECTION("matrix += matrix")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        Matr rhs{ r, c, double(r - c) };

                        lhs += rhs;

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
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
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };

                        lhs += double(r * c);

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
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

TEST_CASE("matrix::operator+", "[matrix]")
{
    SECTION("matrix + matrix")
    {
        SECTION("valid")
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

                    for (std::size_t n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (std::size_t n_c = 0; n_c < result.n_cols(); n_c++)
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
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };

                    Matr result = lhs + double(r * c);

                    for (std::size_t n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (std::size_t n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) + double(r * c));
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("matrix::operator-=", "[matrix]")
{
    SECTION("matrix -= matrix")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        Matr rhs{ r, c, double(r - c) };

                        lhs -= rhs;

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
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

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
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

TEST_CASE("matrix::operator-", "[matrix]")
{
    SECTION("matrix - matrix")
    {
        SECTION("valid")
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

                    for (std::size_t n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (std::size_t n_c = 0; n_c < result.n_cols(); n_c++)
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
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    Matr lhs{ r, c, double(r + c) };

                    Matr result = lhs - double(r * c);

                    for (std::size_t n_r = 0; n_r < result.n_rows(); n_r++)
                    {
                        for (std::size_t n_c = 0; n_c < result.n_cols(); n_c++)
                        {
                            REQUIRE(result(n_r, n_c) == lhs(n_r, n_c) - double(r * c));
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("matrix::operator*=", "[matrix]")
{
    // matrix * matrix
    SECTION("matrix *= double")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };

                        lhs *= double(r * c);

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) * (r * c));
                            }
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("matrix::operator*", "[matrix]")
{
    SECTION("matrix * double")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };

                        lhs * double(r * c);

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) * (r * c));
                            }
                        }
                    }
                }
            }
        }
    }

    SECTION("matrix * matrix")
    {
        SECTION("valid")
        {

        }
    }
}

TEST_CASE("matrix::operator/=", "[matrix]")
{
    SECTION("matrix /= double")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        auto rhs = double(r * c);
                        lhs /= rhs;

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) / rhs);
                            }
                        }
                    }
                }
            }
        }

        SECTION("invalid - division by zero")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) /= 0, MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) /= 0, MatrixError);
        }
    }
}

TEST_CASE("matrix::operator/", "[matrix]")
{
    SECTION("matrix / double")
    {
        SECTION("valid")
        {
            std::size_t n_max_dimension{ 10 };

            for (std::size_t r = 1; r < n_max_dimension; r++)
            {
                for (std::size_t c = 1; c < n_max_dimension; c++)
                {
                    SECTION("unit case")
                    {
                        Matr lhs{ r, c, double(r + c) };
                        auto rhs = double(r * c);
                        lhs / rhs;

                        for (std::size_t n_r = 0; n_r < lhs.n_rows(); n_r++)
                        {
                            for (std::size_t n_c = 0; n_c < lhs.n_cols(); n_c++)
                            {
                                REQUIRE(lhs(n_r, n_c) == (r + c) / rhs);
                            }
                        }
                    }
                }
            }
        }

        SECTION("invalid - division by zero")
        {
            REQUIRE_THROWS_AS(Matr(2, 1) / 0, MatrixError);
            REQUIRE_THROWS_AS(Matr(5, 5) / 0, MatrixError);
        }
    }
}

TEST_CASE("matrix::Range", "[matrix::Range]")
{
    SECTION("Range()")
    {
        Matr::Range r{};
        REQUIRE(!r);
        REQUIRE(r.size() == 0);
        REQUIRE_THROWS_AS(r.start(), MatrixError);
        REQUIRE_THROWS_AS(r.end(), MatrixError);
    }

    SECTION("Range(start, end)")
    {
        SECTION("valid")
        {
            SECTION("same start/end")
            {
                Matr::Range r{ 0, 0 };
                REQUIRE(r);
                REQUIRE(r.size() == 1);
                REQUIRE(r.start() == 0);
                REQUIRE(r.end() == 0);
            }

            SECTION("different start/end")
            {
                Matr::Range r{ 2, 5 };
                REQUIRE(r);
                REQUIRE(r.size() == 4);
                REQUIRE(r.start() == 2);
                REQUIRE(r.end() == 5);
            }
        }

        SECTION("invalid")
        {
            REQUIRE_THROWS_AS(Matr::Range( 2, 1 ), MatrixError);            
        }
    }
}