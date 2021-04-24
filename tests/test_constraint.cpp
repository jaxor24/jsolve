#include "test_includes.h"

#include "constraint.h"


TEST_CASE("constraint::constraint(type)", "[constraint]")
{
    SECTION("construction - valid")
    {
        REQUIRE_NOTHROW(jsolve::Constraint{ jsolve::Constraint::Type::LESS });
    }
}

TEST_CASE("constraint::type()", "[constraint]")
{
    SECTION("getter")
    {
        SECTION("less")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS };
            REQUIRE(c.type() == jsolve::Constraint::Type::LESS);
        }

        SECTION("great")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::GREAT };
            REQUIRE(c.type() == jsolve::Constraint::Type::GREAT);
        }

        SECTION("equal")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::EQUAL };
            REQUIRE(c.type() == jsolve::Constraint::Type::EQUAL);
        }
    }
}

TEST_CASE("constraint::rhs()", "[constraint]")
{
    auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS };

    SECTION("getter")
    {
        SECTION("default")
        {
            REQUIRE(c.rhs() == 0.0);
        }
    }

    SECTION("setter")
    {
        c.rhs() = 10;
        REQUIRE(c.rhs() == 10.0);
    }
}

