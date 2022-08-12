#include "test_includes.h"

#include "constraint.h"
#include "variable.h"


TEST_CASE("constraint::constraint()", "[constraint]")
{
    SECTION("valid construction")
    {
        REQUIRE_NOTHROW(jsolve::Constraint{ jsolve::Constraint::Type::LESS, "test_constraint"});
        REQUIRE_NOTHROW(jsolve::Constraint{ jsolve::Constraint::Type::GREAT, "test_constraint" });
    }
}

TEST_CASE("constraint::name()", "[constraint]")
{
    auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "test_name" };
    REQUIRE(c.name() == "test_name");
}

TEST_CASE("constraint::n_created(), constraint::n_alive()", "[constraint]")
{
    REQUIRE(jsolve::Constraint::n_alive() == 0);
    auto n_already_created = jsolve::Constraint::n_created();

    SECTION("create one")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint"};
        REQUIRE(c.n_created() == n_already_created + 1);
        REQUIRE(c.n_alive() == 1);
    }

    n_already_created = jsolve::Constraint::n_created();

    SECTION("create two")
    {
        auto c1 = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto c2 = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        REQUIRE(c2.n_created() == n_already_created + 2);
        REQUIRE(c2.n_alive() == 2);
    }

    n_already_created = jsolve::Constraint::n_created();

    SECTION("create copy")
    {
        auto c1 = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto c2 = c1;
        REQUIRE(c2.n_created() == n_already_created + 2);
        REQUIRE(c2.n_alive() == 2);
    }
}

TEST_CASE("constraint::type()", "[constraint]")
{
    SECTION("getter")
    {
        SECTION("less")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
            REQUIRE(c.type() == jsolve::Constraint::Type::LESS);
        }

        SECTION("great")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::GREAT, "constraint" };
            REQUIRE(c.type() == jsolve::Constraint::Type::GREAT);
        }

        SECTION("equal")
        {
            auto c = jsolve::Constraint{ jsolve::Constraint::Type::EQUAL, "constraint" };
            REQUIRE(c.type() == jsolve::Constraint::Type::EQUAL);
        }
    }
}

TEST_CASE("constraint::rhs()", "[constraint]")
{
    auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };

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
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto v = std::make_unique<jsolve::Variable>(jsolve::Variable::Type::LINEAR, "variable");
        c.add_to_lhs(5.0, v.get());
    }
}

TEST_CASE("constraint::add_to_rhs()", "[constraint]")
{
    SECTION("simple")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto v = std::make_unique<jsolve::Variable>(jsolve::Variable::Type::LINEAR, "variable");
        c.add_to_rhs(5.0, v.get());
    }
}

TEST_CASE("constraint::get_entries()", "[constraint]")
{
    SECTION("one lhs entry")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto v = std::make_unique<jsolve::Variable>(jsolve::Variable::Type::LINEAR, "variable");
        c.add_to_lhs(5.0, v.get());
        REQUIRE(c.get_entries().size() == 1);
        REQUIRE(c.get_entries().at(v.get()) == 5.0);
    }

    SECTION("one rhs entry")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto v = std::make_unique<jsolve::Variable>(jsolve::Variable::Type::LINEAR, "variable");
        c.add_to_rhs(4.0, v.get());
        REQUIRE(c.get_entries().size() == 1);
        REQUIRE(c.get_entries().at(v.get()) == -4.0);
    }

    SECTION("same variable rhs and lhs")
    {
        auto c = jsolve::Constraint{ jsolve::Constraint::Type::LESS, "constraint" };
        auto v = std::make_unique<jsolve::Variable>(jsolve::Variable::Type::LINEAR, "variable");
        c.add_to_rhs(-2.0, v.get());
        c.add_to_lhs(4.0, v.get());
        REQUIRE(c.get_entries().size() == 1);
        REQUIRE(c.get_entries().at(v.get()) == 6.0);
    }
}

