#include "work.h"
#include "models.h"

#include "logging.h"

#include "jsolve.h"
#include "mps.h"

#include "matrix.h"

void go(std::filesystem::path file)
{
    auto model{jsolve::read_mps(file)};

    // auto model = models::make_model_7();

    log()->info(model.to_string());

    auto solution{jsolve::solve(model)};

    if (solution)
    {
        jsolve::log_solution(debug_logger(), solution.value());
    }
}
