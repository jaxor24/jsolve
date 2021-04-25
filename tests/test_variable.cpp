#include "test_includes.h"

#include "variable.h"


TEST_CASE("variable::variable()", "[variable]")
{
    SECTION("valid construction")
    {
        REQUIRE_NOTHROW(jsolve::Variable{ jsolve::Variable::Type::LINEAR });
        REQUIRE_NOTHROW(jsolve::Variable{ jsolve::Variable::Type::LINEAR, "test_variable" });
    }
}

TEST_CASE("variable::name()", "[variable]")
{
    auto v = jsolve::Variable{ jsolve::Variable::Type::LINEAR, "test_name" };
    REQUIRE(v.name() == "test_name");
}

TEST_CASE("variable::n_created(), variable::n_alive()", "[variable]")
{
    REQUIRE(jsolve::Variable::n_alive() == 0);
    auto n_already_created = jsolve::Variable::n_created();

    SECTION("create one")
    {
        auto v1 = jsolve::Variable{ jsolve::Variable::Type::LINEAR };
        REQUIRE(v1.n_created() == n_already_created + 1);
        REQUIRE(v1.n_alive() == 1);
    }

    n_already_created = jsolve::Variable::n_created();

    SECTION("create two")
    {
        auto v1 = jsolve::Variable{ jsolve::Variable::Type::LINEAR };
        auto v2 = jsolve::Variable{ jsolve::Variable::Type::LINEAR };
        REQUIRE(v2.n_created() == n_already_created + 2);
        REQUIRE(v2.n_alive() == 2);
    }

    n_already_created = jsolve::Variable::n_created();

    SECTION("create copy")
    {
        auto v1 = jsolve::Variable{ jsolve::Variable::Type::LINEAR };
        auto v2 = v1;
        REQUIRE(v2.n_created() == n_already_created + 2);
        REQUIRE(v2.n_alive() == 2);
    }
}

TEST_CASE("variable::cost()", "[variable]")
{
    auto c = jsolve::Variable{ jsolve::Variable::Type::LINEAR };

    SECTION("getter")
    {
        SECTION("default")
        {
            REQUIRE(c.cost() == 0.0);
        }
    }

    SECTION("setter")
    {
        c.cost() = 10;
        REQUIRE(c.cost() == 10.0);
    }
}

TEST_CASE("variable::upper_bound()", "[variable]")
{
    auto c = jsolve::Variable{ jsolve::Variable::Type::LINEAR };

    SECTION("getter")
    {
        SECTION("default")
        {
            REQUIRE(c.upper_bound() == std::numeric_limits<double>::max());
        }
    }

    SECTION("setter")
    {
        c.upper_bound() = 10;
        REQUIRE(c.upper_bound() == 10.0);
    }
}

TEST_CASE("variable::lower_bound()", "[variable]")
{
    auto c = jsolve::Variable{ jsolve::Variable::Type::LINEAR };

    SECTION("getter")
    {
        SECTION("default")
        {
            REQUIRE(c.lower_bound() == 0);
        }
    }

    SECTION("setter")
    {
        c.lower_bound() = 10;
        REQUIRE(c.lower_bound() == 10.0);
    }
}