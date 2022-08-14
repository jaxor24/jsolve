#include "model.h"

#include <ranges>

namespace jsolve
{
	ModelError::ModelError(const std::string& message)
		:
		std::runtime_error(message)
	{}

	ModelError::~ModelError() = default;

	Model::Model(Sense sense, const std::string& name)
		:
		Counter<Model>{ name },
		m_sense{ sense }
	{}

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

	Variable* Model::make_variable(Variable::Type type, const std::string& name)
	{
		auto [it, result] = m_variables.try_emplace(name, nullptr);

		if (result)
		{
			it->second = std::make_unique<Variable>(type, name);
		}
		else
		{
			throw ModelError(fmt::format("Variable name {} already exists", name));
		}

		return it->second.get();
	}

	Constraint* Model::make_constraint(Constraint::Type type, const std::string& name)
	{
		auto [it, result] = m_constraints.try_emplace(name, nullptr);

		if (result)
		{
			it->second = std::make_unique<Constraint>(type, name);
		}
		else
		{
			throw ModelError(fmt::format("Constraint name {} already exists", name));
		}

		return it->second.get();
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
			const auto& variable = it->second;
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
		for (const auto& [name, constraint] : m_constraints)
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

	const std::map<std::string, std::unique_ptr<Variable>>& Model::get_variables() const
	{
		return m_variables;
	}

	const std::map<std::string, std::unique_ptr<Constraint>>& Model::get_constraints() const
	{
		return m_constraints;
	}

	Variable* Model::get_variable(const std::string& name) const
	{
		auto found = m_variables.find(name);
		return found == std::end(m_variables) ? nullptr : found->second.get();
	}

	Variable* Model::get_variable(std::size_t index) const
	{
		// Added until we get a better way of tracking indices during solve.
		if (m_variables.size() <= index)
		{
			return nullptr;
		}
		else
		{
			auto it = std::begin(m_variables);
			std::advance(it, index);
			return it->second.get();
		}
	}

	Constraint* Model::get_constraint(const std::string& name) const
	{
		auto found = m_constraints.find(name);
		return found == std::end(m_constraints) ? nullptr : found->second.get();
	}

	void Model::remove_variable(const std::string& name)
	{
		[[maybe_unused]] auto result = m_variables.erase(name);
	}

	std::ostream& operator<<(std::ostream& os, const Model& m)
	{
		os << m.to_string();
		return os;
	}

}