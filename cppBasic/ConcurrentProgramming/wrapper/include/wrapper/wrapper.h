#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <thread>

namespace test {

#ifndef VSIZE
    #define VSIZE 0
#endif
static_assert(VSIZE > 0);
// constexpr long long size = 100000000;
constexpr int numThreads = 4;

void init_random_vector(std::vector<int>& randValues) {
    randValues.resize(VSIZE);
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> uniformDist(0, 10);
    for(long long i = 0; i < VSIZE; ++i) {
        randValues[i] = uniformDist(engine);
    }
}

template<typename T, typename ... Args>
void functionWrapper(std::string name, T func, Args&& ... args) {
    auto sta = std::chrono::steady_clock::now();
    auto sum = func(std::forward<Args>(args)...);
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[" << name << "]\tResult:" << sum << "\tTimeCost:" << duration.count() << std::endl;
}

}
