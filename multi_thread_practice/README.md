# 数组求和

代码中需要记忆的点：

- [use of rand() is considered bad](https://stackoverflow.com/questions/52869166/why-is-the-use-of-rand-considered-bad) since rand() is a pseudorandom number generator.

- [generate thread-safe uniform random numbers](https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers)

```c++
int intRand(const int & min, const int & max) {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}
```

- usage of mutex

```c++
std::mutex mutex;    // mutex declaration in somewhere else
/* ... */
{
    std::lock_guard<std::mutex> lock(mutex); 
    /* modify variable in concurrent safe way */
}
```

- usage of atomic

```c++
std::atomic<uint64_t> sum = {};
sum += i;
sum.fetch_add(i, std::memory_order_relaxed);
```

- usage of promise and future

```c++
std::promise<int> promise;
auto& future = promise.get_future();
f(promise); /* where promise.set_value(value) is called */
sum += future.get();
```
