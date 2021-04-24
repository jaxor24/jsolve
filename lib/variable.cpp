#include "variable.h"
#include "logging.h"

#include <iostream>

namespace jsolve
{
	std::ostream& operator<<(std::ostream& os, const Variable& v)
	{
		os << '\n';
		os << v.to_string();
		return os;
	};

	Variable::Type Variable::type() const
	{
		return m_type;
	}

	double Variable::cost() const
	{
		return m_cost;
	}

	double& Variable::cost()
	{
		return m_cost;
	}

	std::string Variable::to_string() const
	{
		return fmt::format("{}", "x");
	}
}