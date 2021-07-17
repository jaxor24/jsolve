#include "test_includes.h"

#include "matrix_range.h"
#include "matrix_error.h"

TEST_CASE("Range", "[matrix]")
{
    SECTION("Range()")
    {
        Range r{};
        REQUIRE(!r);
        REQUIRE(r.size() == 0);
        REQUIRE_THROWS_AS(r.start(), MatrixError);
        REQUIRE_THROWS_AS(r.end(), MatrixError);
    }

    SECTION("Range(value)")
    {
        SECTION("valid")
        {
            Range r{ 1 };
            REQUIRE(r);
            REQUIRE(r.size() == 1);
            REQUIRE(r.start() == 1);
            REQUIRE(r.end() == 1);
        }


        SECTION("invalid")
        {
            // todo     
        }
    }

    SECTION("Range(start, end)")
    {
        SECTION("valid")
        {
            SECTION("same start/end")
            {
                Range r{ 0, 0 };
                REQUIRE(r);
                REQUIRE(r.size() == 1);
                REQUIRE(r.start() == 0);
                REQUIRE(r.end() == 0);
            }

            SECTION("different start/end")
            {
                Range r{ 2, 5 };
                REQUIRE(r);
                REQUIRE(r.size() == 4);
                REQUIRE(r.start() == 2);
                REQUIRE(r.end() == 5);
            }
        }

        SECTION("invalid")
        {
            // todo     
        }
    }
}
