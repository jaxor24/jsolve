#include "simplex_common.h"

#include <ranges>

namespace jsolve
{

void convert_free_variables(jsolve::Model& model)
{
    // Free variables are replaced with 2 new variables such that:
    // Original = (New Var 1 - New Var 2)
    // The original variable is removed

    auto it_var = std::begin(model.get_variables());
    while (it_var != std::end(model.get_variables()))
    {
        const auto& variable = it_var->second;

        if (variable->lower_bound() == -std::numeric_limits<double>::infinity() &&
            variable->upper_bound() == std::numeric_limits<double>::infinity())
        {
            auto* variable_positive =
                model.make_variable(jsolve::Variable::Type::LINEAR, fmt::format("FREE_{}_POS", variable->name()));
            auto* variable_negative =
                model.make_variable(jsolve::Variable::Type::LINEAR, fmt::format("FREE_{}_NEG", variable->name()));

            variable_positive->cost() = variable->cost();
            variable_negative->cost() = -variable->cost();

            for (const auto& [constraint_name, constraint] : model.get_constraints())
            {
                auto entry = constraint->get_entries().find(variable.get());

                if (entry != std::end(constraint->get_entries()))
                {
                    // Delete the existing entry
                    auto coefficient = entry->second;
                    constraint->get_entries().erase(entry);
                    // Add the two new variables
                    constraint->add_to_lhs(coefficient, variable_positive);
                    constraint->add_to_lhs(-coefficient, variable_negative);
                }
            }

            ++it_var;
            model.remove_variable(variable->name());
        }
        else
        {
            ++it_var;
        }
    }
}

void convert_bounds(jsolve::Model& model)
{
    // Convert variable bounds to constraints
    // Needed until we have a general simplex implementation

    auto it_var = std::begin(model.get_variables());
    while (it_var != std::end(model.get_variables()))
    {
        const auto& variable = it_var->second;

        if (variable->lower_bound() > 0)
        {
            // Lower bounds are converted to constraints
            auto* constraint = model.make_constraint(
                jsolve::Constraint::Type::GREAT, fmt::format("BND_{}_GEQ_{}", variable->name(), variable->lower_bound())
            );
            constraint->rhs() = variable->lower_bound();
            constraint->add_to_lhs(1.0, variable.get());
        }

        if (variable->upper_bound() < std::numeric_limits<double>::infinity())
        {
            // Upper bounds are converted to constraints
            auto* constraint = model.make_constraint(
                jsolve::Constraint::Type::LESS, fmt::format("BND_{}_LEQ_{}", variable->name(), variable->upper_bound())
            );
            constraint->rhs() = variable->upper_bound();
            constraint->add_to_lhs(1.0, variable.get());
        }

        ++it_var;
    }
}

void convert_equality_constraints(jsolve::Model& model)
{
    // Convert equality constraints to a LEQ+GEQ pair
    // Remove the original constraint

    auto it_cons = std::begin(model.get_constraints());

    while (it_cons != std::end(model.get_constraints()))
    {
        const auto& constraint = it_cons->second;

        if (constraint->type() == jsolve::Constraint::Type::EQUAL)
        {
            // Equality constraints are replaced with an LEQ and GEQ
            // TODO need a way of cloning constraints
            auto* geq_constraint = model.make_constraint(
                jsolve::Constraint::Type::GREAT, fmt::format("EQ_CONS_{}_GEQ", constraint->name())
            );
            auto* leq_constraint = model.make_constraint(
                jsolve::Constraint::Type::LESS, fmt::format("EQ_CONS_{}_LEQ", constraint->name())
            );

            geq_constraint->rhs() = constraint->rhs();
            leq_constraint->rhs() = constraint->rhs();

            geq_constraint->get_entries() = constraint->get_entries();
            leq_constraint->get_entries() = constraint->get_entries();

            ++it_cons;
            model.remove_constraint(constraint->name());
        }
        else
        {
            ++it_cons;
        }
    }
}

void convert_geq_to_leq(jsolve::Model& model)
{
    // Convert GEQ constraints to LEQ

    for (auto& [_, constraint] : model.get_constraints())
    {
        if (constraint->type() == jsolve::Constraint::Type::GREAT)
        {
            constraint->type() = jsolve::Constraint::Type::LESS;
            constraint->rhs() *= -1;
            std::for_each(std::begin(constraint->get_entries()), std::end(constraint->get_entries()), [](auto& pair) {
                pair.second *= -1;
            });
        }
    }
}

void convert_to_equality(jsolve::Model& model)
{
    // Convert all inequality constraints to equality by adding slack vars

    for (auto& [_, constraint] : model.get_constraints())
    {
        assert(constraint->type() != Constraint::Type::GREAT); // Impossible from pre-processing

        if (constraint->type() == jsolve::Constraint::Type::EQUAL)
        {
            // Don't do anything - these just get artifical vars in the algorithms
        }
        else if (constraint->type() == jsolve::Constraint::Type::LESS)
        {
            // Add a slack and convert to equality

            auto* slack_variable =
                model.make_variable(jsolve::Variable::Type::LINEAR, fmt::format("SLACK_{}", constraint->name()));

            slack_variable->slack() = true;

            constraint->add_to_lhs(1, slack_variable);
            constraint->type() = jsolve::Constraint::Type::EQUAL;
        }
        else
        {
            throw ModelError("Impossible");
        }
    }

    auto all_equal = std::ranges::all_of(model.get_constraints(), [](const auto& pair) {
        return pair.second->type() == Constraint::Type::EQUAL;
    });

    if (!all_equal)
    {
        throw ModelError("Pre-processing constraints failed");
    }
}

void pre_process_model(jsolve::Model& model)
{
    // Convert the model to the form:
    // max c[t]x
    // Ax = b
    // st x >= 0
    // Steps:
    // Convert free variables to the difference between 2 other non-neg variables
    // Convert variable bounds to constraints (bad idea, but no other choice at the moment)
    // Convert equality constraints to 2 constraints LEQ + GEQ
    // Convert GEQ constraints to LEQ
    // Convert all constraints to EQ via slack variables

    convert_free_variables(model);
    convert_bounds(model);
    convert_equality_constraints(model);
    convert_geq_to_leq(model);
    convert_to_equality(model);
}
} // namespace jsolve