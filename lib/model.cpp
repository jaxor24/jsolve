#include "model.h"

namespace jsolve
{
	Model::Sense Model::sense() const
	{
		return m_sense;
	}

	std::string& Model::objective_name() 
	{
		return m_objective_name;
	}

	const std::string& Model::objective_name() const
	{
		return m_objective_name;
	}

	std::string Model::to_long_string() const
	{
		std::string s = fmt::format(
			"Model: {} ({} constraints, {} variables)",
			name().empty() ? "Unnamed" : name(),
			m_constraints.size(),
			m_variables.size()
		);

		if (m_variables.empty())
		{
			s.append(" Model is empty.");
			return s;
		}

		s.append("\n");
		s.append("Objective: ");

		// Objective
		if (sense() == Sense::MIN)
		{
			s.append("Min");
		}
		else if (sense() == Sense::MAX)
		{
			s.append("Max");
		}
		else
		{
			throw std::runtime_error("Invalid objective sense");
		}

		s.append(" ");

		auto it = std::begin(m_variables);
		while (it != std::end(m_variables))
		{
			const auto& variable = *it;
			if (variable->cost() != 0)
			{
				s.append(" + ");
				s.append(fmt::format("{}*{}", variable->cost(), variable->name()));
			}
			it++;
		}
		s.append("\n");
		s.append("Subject to:");
		s.append("\n");
		// Constraints
		for (const auto& constraint : m_constraints)
		{
			s.append(constraint->to_string());
			s.append("\n");
		}

		return s;
	}

	std::string Model::to_string() const
	{
		std::string s = fmt::format(
			"Model: {} ({}, {} constraints, {} variables)", 
			name().empty() ? "Unnamed" : name(),
			sense() == Sense::MIN ? "MIN" : "MAX",
			m_constraints.size(),
			m_variables.size()
		);

		return s;
	}

	const std::vector<std::unique_ptr<Variable>>& Model::get_variables() const
	{
		return m_variables;
	}

	const std::vector<std::unique_ptr<Constraint>>& Model::get_constraints() const
	{
		return m_constraints;
	}

	Variable* Model::get_variable(const std::string& name) const
	{
		auto found = std::find_if(
			std::begin(m_variables),
			std::end(m_variables),
			[name](const auto& var)
			{ return var->name() == name; }
		);
		return found == std::end(m_variables) ? nullptr : found->get();
	}

	Constraint* Model::get_constraint(const std::string& name) const
	{
		auto found = std::find_if(
			std::begin(m_constraints),
			std::end(m_constraints),
			[name](const auto& cons)
			{ return cons->name() == name; }
		);
		return found == std::end(m_constraints) ? nullptr : found->get();
	}

	std::ostream& operator<<(std::ostream& os, const Model& m)
	{
		os << m.to_string();
		return os;
	}

}