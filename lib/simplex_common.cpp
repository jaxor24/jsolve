#include "simplex_common.h"

namespace jsolve
{

void pre_process_model(jsolve::Model& model)
{
    // Transform model to deal with requirements of current simplex implementations:
    // Convert variable bounds to constraints
    // Convert free variables to 2 other variables
    // Convert equality constraints to 2 constraints LEQ + GEQ

    auto it_var = std::begin(model.get_variables());
    while (it_var != std::end(model.get_variables()))
    {
        const auto& variable = it_var->second;

        if (variable->lower_bound() == -std::numeric_limits<double>::infinity() &&
            variable->upper_bound() == std::numeric_limits<double>::infinity())
        {
            // Free variables are replaced with 2 new variables such that original = (variable_positive -
            // variable_negative)

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
            if (variable->lower_bound() > 0)
            {
                // Lower bounds are converted to constraints
                auto* constraint = model.make_constraint(
                    jsolve::Constraint::Type::GREAT,
                    fmt::format("BND_{}_GEQ_{}", variable->name(), variable->lower_bound())
                );
                constraint->rhs() = variable->lower_bound();
                constraint->add_to_lhs(1.0, variable.get());
            }

            if (variable->upper_bound() < std::numeric_limits<double>::infinity())
            {
                // Upper bounds are converted to constraints
                auto* constraint = model.make_constraint(
                    jsolve::Constraint::Type::LESS,
                    fmt::format("BND_{}_LEQ_{}", variable->name(), variable->upper_bound())
                );
                constraint->rhs() = variable->upper_bound();
                constraint->add_to_lhs(1.0, variable.get());
            }
            ++it_var;
        }
    }

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
} // namespace jsolve