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
        benchmark::DoNotOptimize(mat.slice({}, {rows - 1, rows - 1}));
    }

    state.SetComplexityN(state.range(0));
}

// Register the function as a benchmark
BENCHMARK(bench_jsolve)->RangeMultiplier(2)->Range(1, 1 << 10)->Complexity(benchmark::oN);

BENCHMARK_MAIN();