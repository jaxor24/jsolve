#include <benchmark/benchmark.h>

#include "bench.h"

using Matr = Matrix<double>;

static void bench_jsolve(benchmark::State& state)
{
    auto rows = static_cast<std::size_t>(state.range(0));
    auto cols = static_cast<std::size_t>(state.range(0));
    Matr mat{rows, cols, 1.0};

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(mat.make_transpose());
    }

    state.SetComplexityN(state.range(0));
}

// Register the function as a benchmark
BENCHMARK(bench_jsolve)->DenseRange(1, 1000, 10)->Complexity(benchmark::oN);

BENCHMARK_MAIN();