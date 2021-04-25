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