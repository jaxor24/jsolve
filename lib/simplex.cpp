#include "simplex.h"

#include "variable.h"
#include "constraint.h"

#include "solver_error.h"

#include "tools.h"
#include "matrix.h"

#include "logging.h"

namespace jsolve::simplex
{
	using Mat = Matrix<double>;

	struct MatrixModel
	{
		Mat c;
		Mat A;
		Mat b;
	};

	void verify_matrix_model(MatrixModel model)
	{
		if (model.A.n_rows() != model.b.n_rows())
		{
			throw SolveError("matrix model A row count and b row count different");
		}

		if (model.A.n_cols() != model.c.n_rows())
		{
			throw SolveError("matrix model A col count and b row count different");
		}
	}

	MatrixModel to_matrix_form(const Model& user_model)
	{	
		auto n_user_vars = user_model.get_variables().size();
		auto n_slack_vars = user_model.get_constraints().size();
		auto n_total_vars = n_user_vars + n_slack_vars;

		// Shield against equal constraints (need 2 slacks)
		std::for_each(
			std::begin(user_model.get_constraints()),
			std::end(user_model.get_constraints()),
			[](const auto& constraint) 
			{ 
				if (constraint->type() == Constraint::Type::EQUAL) { throw SolveError("EQUAL constraints unsupported"); }
			}
		);
		
		// Objective vector
		Mat objective{ n_total_vars, 1, 0.0 };
		
		for (const auto& [n_var, var] : enumerate(user_model.get_variables()))
		{
			objective(n_var, 0) = var->cost();
		}

		if (user_model.sense() == Model::Sense::MIN)
		{
			objective *= -1;
		}

		log()->info(objective);

		// b (RHS) vector
		Mat rhs{ n_slack_vars, 1, 0.0 };
		for (const auto& [n_cons, cons] : enumerate(user_model.get_constraints()))
		{
			if (cons->type() == Constraint::Type::GREAT)
			{
				// 7x + 2y >= 5 becomes -7x - 2y <= -5
				rhs(n_cons, 0) = -1 * cons->rhs();
			}
			else
			{
				rhs(n_cons, 0) = cons->rhs();
			}
		}

		log()->info(rhs);

		// A matrix
		Mat a{ n_slack_vars, n_total_vars, 0.0};

		for (const auto& [n_cons, cons] : enumerate(user_model.get_constraints()))
		{
			auto scale = cons->type() == Constraint::Type::GREAT ? -1.0 : 1.0;
			
			// Add original constraint entries
			for (const auto& [n_var, var] : enumerate(user_model.get_variables()))
			{
				auto found_entry = cons->get_entries().find(var.get());

				if (found_entry != std::end(cons->get_entries()))
				{
					a(n_cons, n_var) = scale * found_entry->second;
				}
			}

			// Add slack var coefficient
			a(n_cons, n_user_vars + n_cons) = 1;
		}

		log()->info(a);
		return { objective, a, rhs };
	}

	void simplex_solve(const MatrixModel& model)
	{
		// Follows chapter 6 of "Linear Programming" 2014.

		// Split A into B (basic section) and N (non-basic section)
		// auto n_cons = model.A.n_rows();
		model.A;
	}
}


namespace jsolve
{
	void solve(const Model& user_model)
	{
		using namespace jsolve::simplex;

		auto matrix_model = to_matrix_form(user_model);
		verify_matrix_model(matrix_model);
		simplex_solve(matrix_model);
	}
}