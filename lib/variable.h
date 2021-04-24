#pragma once

#include "counter.h"

#include <string>

namespace jsolve
{
	class Variable : public counter<Variable>
	{
	public:
		enum class Type
		{
			LINEAR
		};

		explicit Variable(Type type, double cost = 0.0)
			:
			m_type{ type },
			m_cost{ cost }
		{}

		Type type() const;

		double cost() const;
		double& cost();

		std::string to_string() const;

		friend std::ostream& operator<<(std::ostream& os, const Variable& v);

	private:
		Type m_type;
		double m_cost{ 0.0 };
	};

	std::ostream& operator<<(std::ostream& os, const Variable& v);
}