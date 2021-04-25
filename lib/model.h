#pragma once

#include "variable.h"
#include "constraint.h"

#include <vector>
#include <memory>

namespace jsolve
{
	class Model
	{
	public:
		enum class Sense
		{
			MAX,
			MIN
		};

		explicit Model(Sense sense)
			:
			m_sense{ sense }
		{}

		template <typename... Args>
		Variable* make_variable(Args&&... args)
		{
			m_variables.push_back(std::make_unique<Variable>(std::forward<Args>(args)...));
			return m_variables.back().get();
		}

		template <typename... Args>
		Constraint* make_constraint(Args&&... args)
		{
			m_constraints.push_back(std::make_unique<Constraint>(std::forward<Args>(args)...));
			return m_constraints.back().get();
		}

	private:
		Sense m_sense{ Sense::MIN };
		std::vector<std::unique_ptr<Variable>> m_variables;
		std::vector<std::unique_ptr<Constraint>> m_constraints;

	};
}
