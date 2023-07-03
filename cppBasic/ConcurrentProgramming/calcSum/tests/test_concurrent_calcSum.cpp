#include <catch2/catch_test_macros.hpp>

#include "calcSum/calcSum.hpp"

using namespace test;

TEST_CASE("check correctness", "[check_the_answer]") {
    std::vector<int> randValues;
    test::init_random_vector(std::ref(randValues));
    uint64_t true_sum = calcUsingNaiveLoop(randValues);
    REQUIRE(calcUsingStdAccumulate(randValues) == true_sum);
    REQUIRE(calcUsingMutex(randValues) == true_sum);
    REQUIRE(calcUsingAtomic(randValues) == true_sum);
    REQUIRE(calcUsingTask(randValues) == true_sum);
}
