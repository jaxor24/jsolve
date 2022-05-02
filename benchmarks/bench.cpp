#include <benchmark/benchmark.h>

#include "bench.h"

using Matr = Matrix<double>;

static void bench_jsolve(benchmark::State& state) 
{
    Matr m1{ 2, 2, 0.0 };
    Matr m2{ 2, 2, 0.0 };

    for (auto _ : state) 
    {
        m1 *= m2;
    }

}
// Register the function as a benchmark
BENCHMARK(bench_jsolve);


BENCHMARK_MAIN();