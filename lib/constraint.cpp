#include "constraint.h"
#include "logging.h"

#include <iostream>

namespace jsolve
{
	std::ostream& operator<<(std::ostream& os, const Constraint& c)
	{
		os << '\n';
		os << c.to_string();
		os << '\n';
		return os;
	};

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

	std::string Constraint::to_string() const
	{
		std::string s = fmt::format("{}[{}]: ", name().empty() ? "c" : name(), n_created());

		if (!m_entries.empty())
		{
			auto it = std::begin(m_entries);
			while (it != std::end(m_entries))
			{
				s.append(fmt::format("{}{}", it->second, it->first->to_string()));
				it++;
				if (it != std::end(m_entries))
				{
					s.append(" + ");
				}
			}
		}

		if (type() == Type::EQUAL)
		{
			s.append(" = ");
		}
		else if (type() == Type::GREAT)
		{
			s.append(" >= ");
		}
		else if (type() == Type::LESS)
		{
			s.append(" <= ");
		}
		else
		{
			throw std::runtime_error("Invalid constraint type");
		}

		s.append(fmt::format("{}", rhs()));

		return s;
	}
}