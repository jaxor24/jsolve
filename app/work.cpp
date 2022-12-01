#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"
#include "mps.h"

#include "matrix.h"

void go(std::filesystem::path file)
{
    auto model{jsolve::read_mps(file)};

    //auto model = models::make_model_27();
    
    std::vector<std::string> cons_to_remove;
    cons_to_remove.push_back("ROW00001");
    cons_to_remove.push_back("ROW00003");
    cons_to_remove.push_back("ROW00004");
    cons_to_remove.push_back("ROW00005");
    cons_to_remove.push_back("ROW00007");
    cons_to_remove.push_back("ROW00008");
    cons_to_remove.push_back("ROW00010");
    cons_to_remove.push_back("ROW00011");
    cons_to_remove.push_back("ROW00013");
    cons_to_remove.push_back("ROW00015");
    cons_to_remove.push_back("ROW00016");
    cons_to_remove.push_back("ROW00018");
    cons_to_remove.push_back("ROW00019");
    cons_to_remove.push_back("ROW00021");
    cons_to_remove.push_back("ROW00022");
    cons_to_remove.push_back("ROW00024");
    cons_to_remove.push_back("ROW00026");
    cons_to_remove.push_back("ROW00027");
    cons_to_remove.push_back("ROW00029");
    cons_to_remove.push_back("ROW00030");
    cons_to_remove.push_back("ROW00032");
    cons_to_remove.push_back("ROW00033");
    cons_to_remove.push_back("ROW00035");
    cons_to_remove.push_back("ROW00037");
    cons_to_remove.push_back("ROW00038");
    cons_to_remove.push_back("ROW00040");
    cons_to_remove.push_back("ROW00041");
    cons_to_remove.push_back("ROW00043");
    cons_to_remove.push_back("ROW00046");

    for (auto& con : cons_to_remove)
    {
        model.remove_constraint(con);
    }

    std::vector<std::string> vars_to_remove;
    vars_to_remove.push_back("COL00005");
    vars_to_remove.push_back("COL00006");
    vars_to_remove.push_back("COL00008");
    vars_to_remove.push_back("COL00009");
    vars_to_remove.push_back("COL00011");
    vars_to_remove.push_back("COL00016");
    vars_to_remove.push_back("COL00017");
    vars_to_remove.push_back("COL00019");
    vars_to_remove.push_back("COL00020");
    vars_to_remove.push_back("COL00022");
    vars_to_remove.push_back("COL00027");
    vars_to_remove.push_back("COL00028");
    vars_to_remove.push_back("COL00030");
    vars_to_remove.push_back("COL00031");
    vars_to_remove.push_back("COL00033");
    vars_to_remove.push_back("COL00038");
    vars_to_remove.push_back("COL00039");
    vars_to_remove.push_back("COL00041");
    vars_to_remove.push_back("COL00044");
 
    for (auto& var : vars_to_remove)
    {
        model.remove_variable(var);
    }

    log()->info(model.to_long_string());

    auto solution{jsolve::solve(model, jsolve::alg_type::REVISED)};

    if (solution)
    {
        jsolve::log_solution(debug_logger(), solution.value());
    }

    for (const auto& [name, con] : model.get_constraints())
    {
        double sum = 0;

        std::string str;

        for (auto& [var, coeff] : con->get_entries())
        {
            if (coeff > 0)
            {
                str += fmt::format(" + {} * {}", coeff, var->name());
            }
            else
            {
                str += fmt::format(" - {} * {}", coeff, var->name());
            }

            sum += coeff * solution->variables.at(var->name());
        }

        log()->debug("Constraint {}: {} = {} ({})", name, str, con->rhs(), sum);
    
    }

}
