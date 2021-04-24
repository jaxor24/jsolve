#pragma once

#include "variable.h"
#include "counter.h"

#include <map>
#include <memory>

namespace jsolve
{
	class Constraint : public counter<Variable>
	{
	public:
		enum class Type
		{
			LESS,
			GREAT,
			EQUAL
		};

		explicit Constraint(Type type)
			:
			m_type{ type }
		{}

		Type type() const;

		double rhs() const;
		double& rhs();

		void add_to_lhs(double coeff, std::shared_ptr<Variable> var);
		void add_to_rhs(double coeff, std::shared_ptr<Variable> var);

	private:
		Type m_type;
		double m_rhs{ 0.0 };
		std::map<std::shared_ptr<Variable>, double> m_entries;
	};
}