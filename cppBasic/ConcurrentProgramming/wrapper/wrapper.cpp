#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <thread>
#include "wrapper/wrapper.h"

namespace test {

// constexpr long long size = 100000000;
// constexpr int numThreads = 4;


// template<typename T, typename ... Args>
// void functionWrapper(std::string name, T func, Args&& ... args) {
//     auto sta = std::chrono::steady_clock::now();
//     auto sum = func(std::forward<Args>(args)...);
//     std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
//     std::cout << "[" << name << "]\tResult:" << sum << "\tTimeCost:" << duration.count() << std::endl;
// }

}
