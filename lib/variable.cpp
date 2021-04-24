#include "variable.h"


namespace jsolve
{
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
}