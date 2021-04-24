#pragma once


namespace jsolve
{
	class Constraint
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

	private:
		Type m_type;
		double m_rhs{ 0.0 };
	};
}