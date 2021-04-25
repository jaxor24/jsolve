#pragma once

#include "variable.h"
#include "constraint.h"
#include "counter.h"

#include <vector>
#include <memory>

namespace jsolve
{
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
			return m_variables.back().get();
		}

		template <typename... Args>
		Constraint* make_constraint(Args&&... args)
		{
			m_constraints.push_back(std::make_unique<Constraint>(std::forward<Args>(args)...));
			return m_constraints.back().get();
		}

		std::string to_string() const;

		friend std::ostream& operator<<(std::ostream& os, const Model& m);

	private:
		Sense m_sense{ Sense::MIN };
		std::vector<std::unique_ptr<Variable>> m_variables;
		std::vector<std::unique_ptr<Constraint>> m_constraints;

	};

	std::ostream& operator<<(std::ostream& os, const Model& m);
}
