#pragma once

#include "variable.h"
#include "logging.h"

#include <map>

namespace jsolve
{
	struct Solution
	{
		double objective;
		std::map<Variable*, double> variables;
	};

	template <typename Log>
	void log_solution(Log log, const Solution& sol)
	{
		log("Optimal solution = {}", sol.objective);
		log("Variable values:");
		for (const auto& [var, value] : sol.variables)
		{
			log("{} = {}", *var, value);
		}
	}
}
