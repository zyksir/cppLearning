#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <thread>
#include <future>

namespace test {

constexpr long long size = 100000000;
constexpr int numThreads = 4;

void init_random_vector(std::vector<int>& randValues) {
    randValues.resize(size);
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> uniformDist(0, 10);
    for(long long i = 0; i < size; ++i) {
        randValues[i] = uniformDist(engine);
    }
}

void calcUsingSingleLoop(const std::vector<int>& randValues) {
    auto sta = std::chrono::steady_clock::now();
    unsigned long long sum = {};
    for(auto num : randValues) sum += num;
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingSingleLoop]\tTime:" << duration.count() << "\tResult:" << sum << std::endl;

    sta = std::chrono::steady_clock::now();
    sum = std::accumulate(randValues.begin(), randValues.end(), 0);
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAccumulate]\tTime:" << duration.count() << "\tResult:" << sum << std::endl;
}

void calcUsingMutexs(const std::vector<int>& randValues) {
    std::mutex myMutex;

    // first we need an estimation about the speed of lock
    auto sta = std::chrono::steady_clock::now();
    unsigned long long sum = {};
    for(auto num : randValues) {
        std::lock_guard<std::mutex> myLockGuard(myMutex);
        sum += num;
    }
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingSingleLockedLoop]\tTime:" << duration.count() << "\tResult:" << sum << std::endl;

    // next let's optimize it
    sta = std::chrono::steady_clock::now();
    sum = 0;
    auto sumUp = [&randValues, &sum, &myMutex](unsigned long long beg, unsigned long long end) {
        unsigned long long temp_sum = 0;
        for(auto it = beg; it < end; ++it) {
            temp_sum += randValues[it];
        }
        std::lock_guard<std::mutex> myLock(myMutex);
        sum += temp_sum;
    };
    std::vector<std::thread> threadArray;
    unsigned long long beg, end, delta=size/numThreads;

    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end= ((i==numThreads-1)? size : beg + delta);
        threadArray.emplace_back(sumUp, beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArray[i].join();
    }
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingLockedMultiThread]\tTime:" << duration.count() << "\tResult:" << sum << std::endl;
}

void calcUsingAtomicLoop(const std::vector<int>& randValues) {
    unsigned long long beg, end, delta=size/numThreads;
    
    /* += */
    auto sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sum1 = {};
    for(auto i : randValues) sum1 += i;
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicLoop(+=)]\tTime:" << duration.count() << "\tResult:" << sum1 << std::endl;

    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sumT1 = {};
    auto sumUp1 = [&randValues, &sumT1](unsigned long long beg, unsigned long long end) {
        unsigned long long temp_sum = 0;
        for(auto it = beg; it < end; ++it) {
            temp_sum += randValues[it];
        }
        sumT1 += temp_sum;
    };
    std::vector<std::thread> threadArray1;
    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end= ((i==numThreads-1)? size : beg + delta);
        threadArray1.emplace_back(sumUp1, beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArray1[i].join();
    }
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicMultiThread(+=)]\tTime:" << duration.count() << "\tResult:" << sumT1 << std::endl;

    /* fetch_add */
    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sum2 = {};
    for(auto i : randValues) sum2.fetch_add(i);
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicLoop(FetchAdd)]\tTime:" << duration.count() << "\tResult:" << sum2 << std::endl;

    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sumT2 = {};
    auto sumUp2 = [&randValues, &sumT2](unsigned long long beg, unsigned long long end) {
        unsigned long long temp_sum = 0;
        for(auto it = beg; it < end; ++it) {
            temp_sum += randValues[it];
        }
        sumT2.fetch_add(temp_sum);

    };
    std::vector<std::thread> threadArray2;
    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end= ((i==numThreads-1)? size : beg + delta);
        threadArray2.emplace_back(sumUp2, beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArray2[i].join();
    }
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicMultiThread(FetchAdd)]\tTime:" << duration.count() << "\tResult:" << sumT2 << std::endl;

    /* relax */
    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sum3 = {};
    for(auto i : randValues) sum3.fetch_add(i, std::memory_order_relaxed);
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicLoop(Relaxed)]\tTime:" << duration.count() << "\tResult:" << sum3 << std::endl;

    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sumT3 = {};
    auto sumUp3 = [&randValues, &sumT3](unsigned long long beg, unsigned long long end) {
        unsigned long long temp_sum = 0;
        for(auto it = beg; it < end; ++it) {
            temp_sum += randValues[it];
        }
        sumT3.fetch_add(temp_sum, std::memory_order_relaxed);
    };
    std::vector<std::thread> threadArray3;
    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end= ((i==numThreads-1)? size : beg + delta);
        threadArray3.emplace_back(sumUp3, beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArray3[i].join();
    }
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicMultiThread(Relaxed)]\tTime:" << duration.count() << "\tResult:" << sumT3 << std::endl;
    
    // notice that in this case, multi-thread doesn't help at all.
    sta = std::chrono::steady_clock::now();
    std::atomic<unsigned long long> sumStupid = {};
    auto sumUpStupid = [&randValues, &sumStupid](unsigned long long beg, unsigned long long end) {
        for(auto it = beg; it < end; ++it) {
            sumStupid += randValues[it];
        }
    };
    std::vector<std::thread> threadArrayStupid;
    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end = ((i==numThreads-1)? size : beg + delta);
        threadArrayStupid.emplace_back(sumUpStupid, beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArrayStupid[i].join();
    }
    duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingAtomicMultiThread(stupidWay)]\tTime:" << duration.count() << "\tResult:" << sumStupid << std::endl;

}

void calcUsingTasks(const std::vector<int>& randValues) {
    unsigned long long beg, end, delta=size/numThreads;
    auto sta = std::chrono::steady_clock::now();
    unsigned long long sum = {};
    auto sumUp = [&randValues](std::promise<unsigned long long>&& prom, unsigned long long beg, unsigned long long end) {
        unsigned long long tmp_sum = 0;
        for(auto it = beg; it < end; ++it) {
            tmp_sum += randValues[it];
        }
        prom.set_value(tmp_sum);
    };
    std::vector<std::future<unsigned long long>> futureVector;
    std::vector<std::thread> threadArray;
    for(int i = 0; i < numThreads; ++i) {
        beg = i*delta;
        end = ((i==numThreads-1)? size : beg + delta);
        std::promise<unsigned long long> promise;
        futureVector.emplace_back(promise.get_future());
        threadArray.emplace_back(sumUp, std::move(promise), beg, end);
    }
    for(int i = 0; i < numThreads; ++i) {
        sum += futureVector[i].get();
    }
    for(int i = 0; i < numThreads; ++i) {
        threadArray[i].join();
    }
    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - sta;
    std::cout << "[calcUsingTasks]\tTime:" << duration.count() << "\tResult:" << sum << std::endl;
}
}
