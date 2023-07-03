#include "calcSum/calcSum.hpp"

std::vector<int> randValues;
int main() {
    test::init_random_vector(std::ref(randValues));
    test::functionWrapper("calcUsingSingleLoop", test::calcUsingNaiveLoop, std::ref(randValues));
    // test::functionWrapper("calcUsingStdAccumulate", test::calcUsingStdAccumulate, std::ref(randValues));
    // test::functionWrapper("calcUsingMutex", test::calcUsingMutex, std::ref(randValues));
    // test::functionWrapper("calcUsingAtomic", test::calcUsingAtomic, std::ref(randValues));
    // test::functionWrapper("calcUsingTasks", test::calcUsingTask, std::ref(randValues));
    return 0;
}