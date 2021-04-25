#include "test_includes.h"

#include "constraint.h"
#include "variable.h"


TEST_CASE("constraint::constraint()", "[constraint]")
{
    SECTION("valid construction")
    {
        REQUIRE_NOTHROW(jsolve::Constraint{ jsolve::Constraint::Type::LESS });
        REQUIRE_NOTHROW(jsolve::Constraint{ jsolve::Constraint::Type::GREAT, "test_constraint" });
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

TEST_CASE("constraint::add_to_lhs()", "[constraint]")
{
    SECTION("simple")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS };
        auto v = std::make_shared<jsolve::Variable>( jsolve::Variable::Type::LINEAR );
        c.add_to_lhs(5.0, v);
    }
}

TEST_CASE("constraint::add_to_rhs()", "[constraint]")
{
    SECTION("simple")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS };
        auto v = std::make_shared<jsolve::Variable>(jsolve::Variable::Type::LINEAR);
        c.add_to_rhs(5.0, v);
    }
}

