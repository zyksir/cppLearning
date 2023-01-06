#include <chrono>
#include <thread>
#include <future>
#include <numeric>
#include "helper.h"

namespace test {

uint64_t calcUsingNaiveLoop(const std::vector<int>& randValues) {
    uint64_t sum = {};
    for(auto num : randValues) sum += num;
    return sum;
}

void functionWrapper(decltype(calcUsingNaiveLoop) calcSumFunction, std::vector<int>& randValues, std::string name) {
    auto sta = std::chrono::steady_clock::now();
    auto sum = calcSumFunction(randValues);
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[" << name << "]\tResult:" << sum << "\tTimeCost:" << duration.count() << std::endl;
}

uint64_t calcUsingStdAccumulate(const std::vector<int>& randValues) {
    unsigned long long sum = {};
    sum = std::accumulate(randValues.begin(), randValues.end(), 0L);
    std::cout << sum << std::endl;
    return sum;
}

uint64_t calcUsingMutex(const std::vector<int>& randValues) {
    std::mutex myMutex;
    uint64_t sum = {};
    auto sumUp = [&randValues, &sum, &myMutex](size_t beg, size_t end) {
        uint64_t temp_sum = {};
        for(size_t i = beg; i < end; ++i) {
            temp_sum += randValues[i];
        }
        std::lock_guard<std::mutex> myLock(myMutex);
        sum += temp_sum;
    };
    size_t size = randValues.size();
    size_t beg = 0, end = 0, delta = size / test::numThreads;
    std::vector<std::thread> threadArray; threadArray.reserve(test::numThreads);
    while (end < size) {
        beg = end;
        end = std::min(beg + delta, size);
        threadArray.emplace_back(sumUp, beg, end);
    }
    for(auto& thread : threadArray) {
        thread.join();
    }
    return sum;
}

uint64_t calcUsingAtomic(const std::vector<int>& randValues) {
    std::mutex myMutex;
    std::atomic<unsigned long long> sum = {};
    auto sumUp = [&randValues, &sum](size_t beg, size_t end) {
        for(size_t i = beg; i < end; ++i) {
            sum += randValues[i];
            // sum.fetch_add(i);
        }
    };
    size_t size = randValues.size();
    size_t beg = 0, end = 0, delta = size / test::numThreads;
    std::vector<std::thread> threadArray; threadArray.reserve(test::numThreads);
    while (end < size) {
        beg = end;
        end = std::min(beg + delta, size);
        threadArray.emplace_back(sumUp, beg, end);
    }
    for(auto& thread : threadArray) {
        thread.join();
    }
    return sum;
}

uint64_t calcUsingTask(const std::vector<int>& randValues) {
    std::mutex myMutex;
    uint64_t sum = {};
    auto sumUp = [&randValues](std::promise<uint64_t>&& prom, size_t beg, size_t end) {
        uint64_t temp_sum = {};
        for(size_t i = beg; i < end; ++i) {
            temp_sum += randValues[i];
        }
        prom.set_value(temp_sum);
    };
    size_t size = randValues.size();
    size_t beg = 0, end = 0, delta = size / test::numThreads;
    std::vector<std::future<uint64_t>> futureArray; futureArray.reserve(test::numThreads);
    std::vector<std::thread> threadArray; threadArray.reserve(test::numThreads);
    while (end < size) {
        beg = end;
        end = std::min(beg + delta, size);
        std::promise<uint64_t> promise;
        futureArray.emplace_back(promise.get_future());
        threadArray.emplace_back(sumUp, std::move(promise), beg, end);
    }
    for(auto& future : futureArray) {
        sum += future.get();
    }
    for(auto& thread : threadArray) {
        thread.join();
    }
    return sum;
}

}

std::vector<int> randValues;
int main() {
    test::init_random_vector(std::ref(randValues));
    test::functionWrapper(test::calcUsingNaiveLoop, std::ref(randValues), "calcUsingSingleLoop");
    test::functionWrapper(test::calcUsingStdAccumulate, std::ref(randValues), "calcUsingStdAccumulate");
    test::functionWrapper(test::calcUsingMutex, std::ref(randValues), "calcUsingMutex");
    test::functionWrapper(test::calcUsingAtomic, std::ref(randValues), "calcUsingAtomic");
    test::functionWrapper(test::calcUsingTask, std::ref(randValues), "calcUsingTasks");

    // test::calcUsingSingleLoop(std::ref(randValues));
    // test::calcUsingMutex(std::ref(randValues));
    // test::calcUsingAtomicLoop(std::ref(randValues));
    // test::calcUsingTasks(std::ref(randValues));
    return 0;
}