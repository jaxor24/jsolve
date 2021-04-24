#include "constraint.h"


namespace jsolve
{
	Constraint::Type Constraint::type() const
	{
		return m_type;
	}

	double Constraint::rhs() const
	{
		return m_rhs;
	}

	double& Constraint::rhs()
	{
		return m_rhs;
	}
}