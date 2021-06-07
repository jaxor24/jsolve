#pragma once

#include "variable.h"
#include "constraint.h"
#include "counter.h"

#include <vector>
#include <set>
#include <memory>
#include <stdexcept>

namespace jsolve
{
	class ModelError : public std::runtime_error
	{
	public:
		template <typename... Args>
		explicit ModelError(Args&&... args)
			:
			std::runtime_error(fmt::format(std::forward<Args>(args)...))
		{}
		virtual ~ModelError() throw () {}
	};


	class Model : public Counter<Model>
	{ 
	public:
		enum class Sense
		{
			MAX,
			MIN
		};

		explicit Model(Sense sense, const std::string& name)
			:
			Counter<Model>{ name },
			m_sense{ sense }
		{}

		Sense sense() const;

		template <typename... Args>
		Variable* make_variable(Args&&... args)
		{
			m_variables.push_back(std::make_unique<Variable>(std::forward<Args>(args)...));
			
			auto* v = m_variables.back().get();

			if (m_variable_names.find(v->name()) == std::end(m_variable_names))
			{
				m_variable_names.insert(v->name());
			}
			else
			{
				throw ModelError("Variable name {} already exists", v->name());
			}

			std::sort(
				std::begin(m_variables), 
				std::end(m_variables), 
				[](const auto& lhs, const auto& rhs) 
				{ return lhs->name() < rhs->name(); }
			);

			return v;
		}

		template <typename... Args>
		Constraint* make_constraint(Args&&... args)
		{
			m_constraints.push_back(std::make_unique<Constraint>(std::forward<Args>(args)...));
			
			auto* c = m_constraints.back().get();
			
			if (m_constraint_names.find(c->name()) == std::end(m_constraint_names))
			{
				m_constraint_names.insert(c->name());
			}
			else
			{
				throw ModelError("Constraint name {} already exists", c->name());
			}

			std::sort(
				std::begin(m_constraints), 
				std::end(m_constraints),
				[](const auto& lhs, const auto& rhs)
				{ return lhs->name() < rhs->name(); }
			);
			
			return c;
		}

		std::string to_string() const;

		const std::vector<std::unique_ptr<Variable>>& get_variables() const;
		const std::vector<std::unique_ptr<Constraint>>& get_constraints() const;

		friend std::ostream& operator<<(std::ostream& os, const Model& m);

	private:
		Sense m_sense{ Sense::MIN };
		std::vector<std::unique_ptr<Variable>> m_variables;
		std::vector<std::unique_ptr<Constraint>> m_constraints;

		std::set<std::string> m_variable_names;
		std::set<std::string> m_constraint_names;

	};

	std::ostream& operator<<(std::ostream& os, const Model& m);
}
