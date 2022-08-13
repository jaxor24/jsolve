#pragma once

#include "variable.h"
#include "logging.h"

#include <map>

namespace jsolve
{
	struct Solution
	{
		double objective{ 0.0 };
		std::map<std::string, double> variables;
	};

	template <typename Log>
	void log_solution(Log log, const Solution& sol)
	{
		log("Optimal solution = {}", sol.objective);
		log("Variable values:");
		for (const auto& [name, value] : sol.variables)
		{
			log("{} = {}", name, value);
		}
	}
}
