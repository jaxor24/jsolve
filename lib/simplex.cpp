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

	struct Var
	{
		std::size_t index;
		bool slack{ false };
		bool dummy{ false };
	};

	struct Locations
	{
		std::map<std::size_t, Var> basics;
		std::map<std::size_t, Var> non_basics;
	};

	void pivot(std::size_t pivot_row, std::size_t pivot_col, Locations& locations, double& obj, Mat& A, Mat& b, Mat& c)
	{
		// Extract rows and cols from A
		auto Acol = A.slice({}, { pivot_col });
		auto Arow = A.slice({ pivot_row }, {});
		auto a = A(pivot_row, pivot_col);

		// Pivot A
		A = A - ((Acol * Arow) / a);
		A.update(pivot_row, {}, -1 * Arow / a);
		A.update({}, pivot_col, +1 * Acol / a);
		A(pivot_row, pivot_col) = 1 / a;

		// Pivot b
		auto brow = b(pivot_row, 0);
		b = b - brow * Acol / a;
		b(pivot_row, 0) = -1.0 * brow / a;

		// Pivot c
		auto ccol = c(0, pivot_col);
		auto s = ccol * Arow / a;
		c = c - ccol * Arow / a;
		c(0, pivot_col) = ccol / a;

		// Update objective
		obj = obj - ccol * brow / a;

		// Update var location
		auto tmp = locations.basics[pivot_row];
		auto tmp1 = locations.non_basics[pivot_col];

		locations.basics[pivot_row] = tmp1;
		locations.non_basics[pivot_col] = tmp;
	}

	Locations init_locations(const Mat& A, bool phase_1_dummy)
	{
		Locations locations;

		// Initially, rows of A are the slack vars
		for (std::size_t i = 0; i < A.n_rows(); i++)
		{
			locations.basics[i] = { i, true, false };
		}

		// Initially, cols of A are the user vars
		for (std::size_t i = 0; i < A.n_cols(); i++)
		{
			locations.non_basics[i] = { i, false, false };
		}
		
		// Assume phase 1 dummy
		if (phase_1_dummy)
		{
			std::size_t idx = A.n_cols() - 1;
			locations.non_basics[idx] = { idx, false, true };
		}

		return locations;
	}

	std::optional<Solution> primal_solve(const Model& user_model)
	{
		// Follows the implementation in Chapter 4 p46. of "Linear Programming" 2014.

		Timer timer{ info_logger(),  "Primal Simplex Algorithm" };

		auto model = to_standard_form(user_model);

		auto c = model.c;
		auto A = -1 * model.A;  // -1 to model rearrange for slack vars
		auto b = model.b;
		
		// Keep track of variables so we can recover solutions at the end
		auto locations = init_locations(A, in_phase_1);

		int max_iter = 20;
		int iter = 1;
		double obj = 0;
		double eps = 1e-4;

		while (c.row_max().first(0, 0) > eps)
		{
			log()->debug("---------------------------------------");
			log()->debug("Iteration: {}", iter);
			//log()->info("c = {}", c);
			//log()->info("A = {}", A);
			//log()->info("b = {}", b);

			// Pick largest objective coefficient
			auto [c_max, c_max_index] = c.row_max();

			auto col_idx = c_max_index(0, 0);
			log()->debug("Entering variable:");
			log()->debug("Objective max coeff {} at col {}", c_max(0, 0), col_idx);
			// Grab the corresponding A column
			auto Acol = A.slice({}, { col_idx });

			// Select leaving variable
			// Pick i such that: a[i,k]/b[i] is maximised
			auto [t, leaving_row_index] = div_elem(-1.0 * Acol, b).col_max();
			auto leaving_ratio = t(0, 0);
			auto row_idx = leaving_row_index(0, 0);

			log()->debug("Leaving variable:");
			log()->debug("With a max ratio value {} at row {}", t(0, 0), row_idx);

			if (leaving_ratio < eps)
			{
				// Leaving ratio is non-positive
				log()->warn("Model is unbounded");
				return {};
			}

			log()->debug("Pivot on element {} at {},{}", A(row_idx, col_idx), row_idx, col_idx);

			pivot(row_idx, col_idx, locations, obj, A, b, c);

			iter++;
			if (iter == max_iter)
			{
				log()->warn("Iteration limit ({}) reached.", iter);
				return {};
			}
		}

		if (iter == 1)
		{
			log()->warn("Zero iterations. Error in model.", iter);
			return {};
		}

		log()->debug("---------------------------------------");
		log()->info("Optimal solution = {} ({} iterations)", obj, iter);

		// Extract solution
		Solution sol;
		sol.objective = obj;

		for (auto& [idx, var] : locations.basics)
		{ 
			if (!var.slack)
			{
				sol.variables[user_model.get_variables().at(var.index)->name()] = b(idx, 0);
			}
		}

		for (auto& [idx, var] : locations.non_basics)
		{ 
			if (!var.slack)
			{
				sol.variables[user_model.get_variables().at(var.index)->name()] = 0;
			}
		}

		return sol;
	}
}
