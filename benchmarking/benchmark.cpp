#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_constructor.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../include/search.hpp"
#include "../include/board.hpp"

using namespace n_brd;
using namespace n_sch;
TEST_CASE("main") {
    printf("entering testcase");
    BENCHMARK_ADVANCED("search")(Catch::Benchmark::Chronometer meter) {
        printf("entering benchmark");
        board b = board();
        meter.measure([&b] { return search(b); });
    };
}
