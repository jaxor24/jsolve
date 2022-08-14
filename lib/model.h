#pragma once

#include "variable.h"
#include "constraint.h"
#include "counter.h"

#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace jsolve
{
	class ModelError : public std::runtime_error
	{
	public:
		explicit ModelError(const std::string& message);
		virtual ~ModelError();
	};

	class Model : public Counter<Model>
	{ 
	public:
		enum class Sense
		{
			MAX,
			MIN
		};

		explicit Model(Sense sense, const std::string& name);

		Sense sense() const;

		std::string& objective_name();
		const std::string& objective_name() const;

		Variable* make_variable(Variable::Type type, const std::string& name);
		Constraint* make_constraint(Constraint::Type type, const std::string& name);

		Variable* get_variable(const std::string& name) const;
		Variable* get_variable(std::size_t index) const;

		Constraint* get_constraint(const std::string& name) const;

		void remove_variable(const std::string& name);

		std::string to_string() const;
		std::string to_long_string() const;

		const std::map<std::string, std::unique_ptr<Variable>>& get_variables() const;
		const std::map<std::string, std::unique_ptr<Constraint>>& get_constraints() const;

		friend std::ostream& operator<<(std::ostream& os, const Model& m);

	private:
		Sense m_sense{ Sense::MIN };

		std::string m_objective_name;

		std::map<std::string, std::unique_ptr<Variable>> m_variables;
		std::map<std::string, std::unique_ptr<Constraint>> m_constraints;
	};

	std::ostream& operator<<(std::ostream& os, const Model& m);
}
