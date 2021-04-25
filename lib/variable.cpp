#include "variable.h"
#include "logging.h"

#include <iostream>

namespace jsolve
{
	std::ostream& operator<<(std::ostream& os, const Variable& v)
	{
		os << v.to_string();
		return os;
	}

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

	double Variable::upper_bound() const
	{
		return m_upper_bound;
	}

	double& Variable::upper_bound()
	{
		return m_upper_bound;
	}

	double Variable::lower_bound() const
	{
		return m_lower_bound;
	}

	double& Variable::lower_bound()
	{
		return m_lower_bound;
	}

	std::string Variable::to_string() const
	{
		return fmt::format("{}", name().empty() ? fmt::format("v[{}]", n_created()) : name());
	}
}