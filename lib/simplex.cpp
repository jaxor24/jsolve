#include "simplex.h"

#include "variable.h"
#include "constraint.h"

#include "solver_error.h"

#include "tools.h"
#include "matrix.h"

#include "logging.h"

#include <algorithm>

namespace jsolve
{
	using Number = double;
	using Mat = Matrix<Number>;

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

		// Create a new constraint vector with the equality constraints converted to LESS and GREAT pairs.
		std::vector<Constraint> converted_constraints;
		for (const auto& [n_cons, cons] : enumerate(user_model.get_constraints()))
		{
			if (cons->type() == Constraint::Type::EQUAL)
			{
				converted_constraints.emplace_back(*cons.get());
				converted_constraints.back().type() = Constraint::Type::GREAT;

				converted_constraints.emplace_back(*cons.get());
				converted_constraints.back().type() = Constraint::Type::LESS;
			}
			else
			{
				converted_constraints.emplace_back(*cons.get());
			}
		}

		auto n_user_vars = user_model.get_variables().size();
		auto n_user_cons = converted_constraints.size();

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
		for (const auto& [n_cons, cons] : enumerate(converted_constraints))
		{
			if (cons.type() == Constraint::Type::GREAT)
			{
				// 7x + 2y >= 5 becomes -7x - 2y <= -5
				rhs(n_cons, 0) = -1 * cons.rhs();
			}
			else if (cons.type() == Constraint::Type::LESS)
			{
				rhs(n_cons, 0) = cons.rhs();
			}
			else
			{
				// Impossible by construction of vector being iterated.
			}
		}

		// A matrix
		Mat a{ n_user_cons, n_user_vars, 0.0 };

		for (const auto& [n_cons, cons] : enumerate(converted_constraints))
		{
			auto scale = cons.type() == Constraint::Type::GREAT ? -1.0 : 1.0;

			// Add original constraint entries
			for (const auto& [n_var, var] : enumerate(user_model.get_variables()))
			{
				auto found_entry = cons.get_entries().find(var.get());

				if (found_entry != std::end(cons.get_entries()))
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

	void pivot(std::size_t pivot_row, std::size_t pivot_col, Locations& locations, double& obj_phase_1, double& obj_phase_2, Mat& A, Mat& b, Mat& c_phase_1, Mat& c_phase_2)
	{
		// We can optionally pivot on 2 objectives at the same time
		// If a phase 1 objective is passed, the objective will be updated using it.
		
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

		// Pivot phase 1 objective
		{
			auto ccol = c_phase_1(0, pivot_col);
			auto s = ccol * Arow / a;
			c_phase_1 = c_phase_1 - ccol * Arow / a;
			c_phase_1(0, pivot_col) = ccol / a;
			obj_phase_1 = obj_phase_1 - ccol * brow / a;
		}

		// Pivot phase 2 objective
		{
			auto ccol = c_phase_2(0, pivot_col);
			auto s = ccol * Arow / a;
			c_phase_2 = c_phase_2 - ccol * Arow / a;
			c_phase_2(0, pivot_col) = ccol / a;
			obj_phase_2 = obj_phase_2 - ccol * brow / a;
		}

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

	std::optional<std::size_t> get_entering_variable(Mat& c, bool in_phase_1, Locations& locations, double eps)
	{
		// Return the index in c of the entering variable.
		
		if (in_phase_1)
		{
			// In phase 1 we can pivot on any variable in c
			auto [c_max, c_max_index] = c.row_max();
			if (c_max(0, 0) > eps)
			{
				return c_max_index(0, 0);
			}
			else
			{
				return {};
			}
		}
		else
		{
			// In phase 2 we can only pivot on the non-dummy variables
			// TODO - Write Matrix::iterator then replace with enumerate or range() code
			
			std::size_t c_max_index{ 0 };
			Mat::value_type c_max_value{ eps };

			for (std::size_t curr_idx = 0; curr_idx < c.n_cols(); curr_idx++)
			{
				if (!locations.non_basics.at(curr_idx).dummy && (c(0, curr_idx) > c_max_value))
				{
					c_max_value = c(0, curr_idx);
					c_max_index = curr_idx;
				}
			}

			if (c_max_value > eps)
			{
				return c_max_index;
			}
			else
			{
				return {};
			}
		}
	}

	auto ratio_test_division = [](Number a, Number b) -> Number
	{
		// A division operator that includes the conventions for the simplex ratio test:
		// a / 0 = infinity
		// 0 / 0 = 0

		auto a_zero = approx_equal(a, 0.0);
		auto b_zero = approx_equal(b, 0.0);

		if (a_zero && b_zero)
		{
			return 0.0;
		}
		else if (b_zero)
		{
			if (a > 0)
			{
				return std::numeric_limits<Number>::infinity();
			}
			else
			{
				return -1 * std::numeric_limits<Number>::infinity();
			}
		}
		else
		{
			return a / b;
		}
	};

	std::optional<Solution> primal_solve(const Model& user_model)
	{
		// Follows the implementation in Chapter 4 p46. of "Linear Programming" 2014.

		Timer timer{ info_logger(),  "Primal Simplex Algorithm" };

		auto model = to_standard_form(user_model);

		// Assume we need both phase 1 and 2 objectives

		// Add dummy variable as variable n + 1 (extra column on A and c)
		// 
		// Phase 1 objective is = max [ 0 | -1]
		auto c_phase_1 = Mat{ model.c.n_rows(), model.c.n_cols() + 1, 0.0 };
		c_phase_1(0, c_phase_1.n_cols() - 1) = -1;

		// Phase 2 objective  = [ c | 0 ]
		auto c_phase_2 = Mat{ model.c.n_rows(), model.c.n_cols() + 1, 0.0 };
		c_phase_2.update({}, { 0, model.c.n_cols() - 1 }, model.c);

		// Add dummy variable to all constraints
		auto A_phase_1 = Mat{ model.A.n_rows(), model.A.n_cols() + 1, 0.0 };
		A_phase_1.update({ 0, model.A.n_rows() - 1 }, { 0, model.A.n_cols() - 1 }, model.A);
		A_phase_1.update({}, { model.A.n_cols() }, Mat{ model.A.n_rows(), 1, -1.0 });

		auto A = -1 * A_phase_1;  // -1 to model rearranging for slack vars
		auto b = model.b;

		bool in_phase_1 = false;

		// Determine if phase 1 is needed
		auto [b_mins, b_min_indices] = model.b.col_min();
		if (b_mins(0, 0) < 0)
		{
			in_phase_1 = true;
		}

		// Keep track of variables so we can recover solutions at the end
		auto locations = init_locations(A, true);

		double obj_phase_1 = 0;
		double obj_phase_2 = 0;

		// Do first pivot of phase 1 variable and most negative RHS row
		if (in_phase_1)
		{
			auto [row_min, row_min_idx] = b.col_min();
			auto col_idx = c_phase_1.n_cols() - 1;
			pivot(
				row_min_idx(0,0),
				col_idx, 
				locations, 
				obj_phase_1, 
				obj_phase_2, 
				A, 
				b, 
				c_phase_1, 
				c_phase_2
			);
		}

		// Pick the objective we need to use
		auto& c = in_phase_1 ? c_phase_1 : c_phase_2;
		
		int max_iter = 50;
		int iter = 1;
		double eps = 1e-4;

		auto entering_idx = get_entering_variable(c, in_phase_1, locations, eps);

		while (entering_idx)
		{
			log()->debug("---------------------------------------");
			log()->debug("Iteration: {}", iter);
			log()->debug("Phase 1 obj = {} {}", obj_phase_1, in_phase_1 ? "***" : "");
			log()->debug("Phase 2 obj = {} {}", obj_phase_2, in_phase_1 ? "" : "***");
			log()->debug("c = {}", c);
			log()->debug("A = {}", A);
			log()->debug("b = {}", b);

			auto col_idx = entering_idx.value();
			log()->debug("Entering variable:");
			log()->debug("Objective max coeff {} at col {}", c(0, col_idx), col_idx);
			// Grab the corresponding A column
			auto Acol = A.slice({}, { col_idx });

			// Select leaving variable
			// Pick i such that: a[i,k]/b[i] is maximised
			auto [t, leaving_row_index] = div_elem(-1.0 * Acol, b, ratio_test_division).col_max();
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

			pivot(
				row_idx, 
				col_idx, 
				locations, 
				obj_phase_1, 
				obj_phase_2, 
				A, 
				b, 
				c_phase_1, 
				c_phase_2
			);

			// Update our loop vars
			entering_idx = get_entering_variable(c, in_phase_1, locations, eps);

			// Switch to phase 2 objective
			if (in_phase_1 && !entering_idx)
			{
				in_phase_1 = false;
				c = c_phase_2;
				entering_idx = get_entering_variable(c, in_phase_1, locations, eps);
			}

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
		log()->info("Optimal solution = {} ({} iterations)", obj_phase_2, iter);

		// Extract solution
		Solution sol;
		sol.objective = obj_phase_2;

		for (auto& [idx, var] : locations.basics)
		{ 
			if (!var.slack && !var.dummy)
			{
				sol.variables[user_model.get_variables().at(var.index)->name()] = b(idx, 0);
			}
		}

		for (auto& [idx, var] : locations.non_basics)
		{ 
			if (!var.slack && !var.dummy)
			{
				sol.variables[user_model.get_variables().at(var.index)->name()] = 0;
			}
		}

		return sol;
	}
}
