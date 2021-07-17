#include "test_includes.h"

#include "matrix.h"

using Matr = Matrix<double>;

TEST_CASE("matrix::iterator", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m1{ 1, 1, 1.0 };

        SECTION("++")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1.0);
            it++;
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_row()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1.0);
            it.next_row();
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_col()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1.0);
            it.next_col();
            REQUIRE(it == std::end(m1));
        }
    }

    SECTION("2x2 matrix")
    {
        Matr m1{ 2, 2, 0.0 };  // [1 2; 3 4]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        SECTION("++")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(*it == 4);
            it++;
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_row()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(*it == 3);
            it.next_row();
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_col()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(*it == 2);
            it.next_col();
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_row() + next_col()")
        {
            Matr::value_type curr = 1;
            for (auto it_row = std::begin(m1); it_row != std::end(m1); it_row.next_row())
            {
                for (auto it_col = it_row; it_col != std::end(m1); it_col.next_col())
                {
                    REQUIRE(*it_col == curr);
                    curr++;
                }
            }
        }
    }

    SECTION("1x3 vector")
    {
        Matr m1{ 1, 3 }; // [1 2 3]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(0, 2) = 3;

        SECTION("++")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_row()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_col()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(*it == 2);
            it.next_col();
            REQUIRE(*it == 3);
            it.next_col();
            REQUIRE(it == std::end(m1));
        }
    }

    SECTION("3x1 vector")
    {
        Matr m1{ 3, 1 }; // [1;2;3]
        m1(0, 0) = 1;
        m1(1, 0) = 2;
        m1(2, 0) = 3;

        SECTION("++")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_row()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(*it == 2);
            it.next_row();
            REQUIRE(*it == 3);
            it.next_row();
            REQUIRE(it == std::end(m1));
        }

        SECTION("next_col()")
        {
            auto it = std::begin(m1);
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(it == std::end(m1));
        }
    }
}

TEST_CASE("matrix::const_iterator", "[matrix]")
{
    SECTION("1x1 matrix")
    {
        Matr m1{ 1, 1, 1.0 };

        SECTION("++")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1.0);
            it++;
            REQUIRE(it == m1.cend());
        }

        SECTION("next_row()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1.0);
            it.next_row();
            REQUIRE(it == m1.cend());
        }

        SECTION("next_col()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1.0);
            it.next_col();
            REQUIRE(it == m1.cend());
        }
    }

    SECTION("2x2 matrix")
    {
        Matr m1{ 2, 2, 0.0 };  // [1 2; 3 4]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(1, 0) = 3;
        m1(1, 1) = 4;

        SECTION("++")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(*it == 4);
            it++;
            REQUIRE(it == m1.cend());
        }

        SECTION("next_row()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(*it == 3);
            it.next_row();
            REQUIRE(it == m1.cend());
        }

        SECTION("next_col()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(*it == 2);
            it.next_col();
            REQUIRE(it == m1.cend());
        }

        SECTION("next_row() + next_col()")
        {
            Matr::value_type curr = 1;
            for (auto it_row = m1.cbegin(); it_row != m1.cend(); it_row.next_row())
            {
                for (auto it_col = it_row; it_col != m1.cend(); it_col.next_col())
                {
                    REQUIRE(*it_col == curr);
                    curr++;
                }
            }
        }
    }

    SECTION("1x3 vector")
    {
        Matr m1{ 1, 3 }; // [1 2 3]
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(0, 2) = 3;

        SECTION("++")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(it == m1.cend());
        }

        SECTION("next_row()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(it == m1.cend());
        }

        SECTION("next_col()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(*it == 2);
            it.next_col();
            REQUIRE(*it == 3);
            it.next_col();
            REQUIRE(it == m1.cend());
        }
    }

    SECTION("3x1 vector")
    {
        Matr m1{ 3, 1 }; // [1;2;3]
        m1(0, 0) = 1;
        m1(1, 0) = 2;
        m1(2, 0) = 3;

        SECTION("++")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it++;
            REQUIRE(*it == 2);
            it++;
            REQUIRE(*it == 3);
            it++;
            REQUIRE(it == m1.cend());
        }

        SECTION("next_row()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_row();
            REQUIRE(*it == 2);
            it.next_row();
            REQUIRE(*it == 3);
            it.next_row();
            REQUIRE(it == m1.cend());
        }

        SECTION("next_col()")
        {
            auto it = m1.cbegin();
            REQUIRE(*it == 1);
            it.next_col();
            REQUIRE(it == m1.cend());
        }
    }
}
