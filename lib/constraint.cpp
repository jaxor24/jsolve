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

	void Constraint::add_to_lhs(double coeff, std::shared_ptr<Variable> var)
	{
		m_entries[var] += coeff;
	}

	void Constraint::add_to_rhs(double coeff, std::shared_ptr<Variable> var)
	{
		add_to_lhs(-1 * coeff, var);
	}
}