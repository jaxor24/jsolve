#include <benchmark/benchmark.h>

#include "bench.h"

using Matr = Matrix<double>;

static void bench_jsolve(benchmark::State& state) 
{
    for (auto _ : state) 
    {
        state.PauseTiming();

        auto rows = static_cast<std::size_t>(state.range(0));
        auto cols = static_cast<std::size_t>(state.range(0));

        /*std::cout << rows << "," << cols << std::endl;*/

        Matr m1{ rows, cols, 1.0 };
        Matr m2{ rows, cols, 2.0 };

        state.ResumeTiming();

        m1 *= m2;
    }

    state.SetComplexityN(state.range(0));

}
// Register the function as a benchmark
BENCHMARK(bench_jsolve)->RangeMultiplier(2)->Range(1, 1 << 9)->Complexity(benchmark::oN);


BENCHMARK_MAIN();