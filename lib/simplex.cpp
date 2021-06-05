#include "simplex.h"

#include "variable.h"
#include "constraint.h"

#include "solver_error.h"

#include "tools.h"
#include "matrix.h"

#include "logging.h"

namespace jsolve
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

}

namespace jsolve::simplex
{
	MatrixModel to_standard_form(const Model& user_model)
	{
		// Returns A, b and c such that:
		// - objective is maximisation
		// - constaints are LHS <= RHS
		// - vars are non-negative
		// With:
		// A = n_cons * n_vars
		// b = n_cons * 1,
		// c = 1 * n_vars

		auto n_user_vars = user_model.get_variables().size();
		auto n_user_cons = user_model.get_constraints().size();

		// Shield against equal constraints
		std::for_each(
			std::begin(user_model.get_constraints()),
			std::end(user_model.get_constraints()),
			[](const auto& constraint)
			{
				if (constraint->type() == Constraint::Type::EQUAL) { throw SolveError("EQUAL constraints unsupported"); }
			}
		);

		// Objective vector
		Mat objective{ 1, n_user_vars, 0.0 };

		for (const auto& [n_var, var] : enumerate(user_model.get_variables()))
		{
			objective(0, n_var) = var->cost();
		}

		if (user_model.sense() == Model::Sense::MIN)
		{
			objective *= -1;
		}

		// b (RHS) vector
		Mat rhs{ n_user_cons, 1, 0.0 };
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

		// A matrix
		Mat a{ n_user_cons, n_user_vars, 0.0 };

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
		}

		//log()->info(objective);
		//log()->info(rhs);
		//log()->info(a);
		return { objective, a, rhs };
	}

	void primal_solve(const Model& user_model)
	{
		// Follows the implementation in Chapter 4 p46. of "Linear Programming" 2014.
		
		log()->info("Primal Simplex Algorithm (component form)");

		auto model = to_standard_form(user_model);

		auto c = model.c;
		auto A = -1 * model.A;  // -1 to model rearrange for slack vars
		auto b = model.b;
		double eps = 1e-4;

		int iter = 1;
		int max_iter = 20;
		double obj = 0;

		while (c.row_max().first(0, 0) > eps)
		{
			log()->info("---------------------------------------");
			log()->info("Iteration: {}", iter);
			//log()->info("c = {}", c);
			//log()->info("A = {}", A);
			//log()->info("b = {}", b);

			// Pick largest objective coefficient
			auto [c_max, c_max_index] = c.row_max();

			auto col_idx = c_max_index(0, 0);
			log()->info("Entering variable:");
			log()->info("Objective max coeff {} at col {}", c_max(0, 0), col_idx);
			// Grab the corresponding A column
			auto Acol = A.slice({}, { col_idx, col_idx });

			// Select leaving variable
			// Pick i such that: a[i,k]/b[i] is maximised
			auto [t, leaving_row_index] = div_elem(-1.0 * Acol, b).col_max();
			auto leaving_ratio = t(0, 0);
			auto row_idx = leaving_row_index(0, 0);

			log()->info("Leaving variable:");
			log()->info("With a max ratio value {} at row {}", t(0, 0), row_idx);

			// Test for unbounded-ness
			// Leaving ratio is non-positive
			if (leaving_ratio < eps)
			{
				// Unbounded
				log()->info("unbounded");
				return;
			}

			log()->info("Pivot on element {} at {},{}", A(row_idx, col_idx), row_idx, col_idx);

			auto Arow = A.slice({ row_idx }, {});
			auto a = A(row_idx, col_idx);

			// Pivot A
			A = A - ((Acol * Arow) / a);
			A.update(row_idx, {}, -1 * Arow / a);
			A.update({}, col_idx, +1 * Acol / a);
			A(row_idx, col_idx) = 1 / a;

			// Pivot b
			auto brow = b(row_idx, 0);
			b = b - brow * Acol / a;
			b(row_idx, 0) = -1.0 * brow / a;

			// Pivot c
			auto ccol = c(0, col_idx);
			auto s = ccol * Arow / a;
			c = c - ccol * Arow / a;
			c(0, col_idx) = ccol / a;

			// Update objective
			obj = obj - ccol * brow / a;

			iter++;
			if (iter == max_iter)
			{
				log()->info("Iteration limit ({}) reached.", iter);
				break;
			}
		}
		log()->info("---------------------------------------");
		log()->info("Optimal solution = {}", obj);
	}
}
