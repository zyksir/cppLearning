#include <iostream>
#include <thread>
#include <future>
#include "SafeQueue.h"

class ThreadPool {
private:
    class ThreadWorker {
    private:
        int m_id;
        ThreadPool * m_pool;
    public:
        ThreadWorker(ThreadPool * pool, const int id):m_pool(pool),m_id(id){}
        void operator()();
    };

    bool m_shutdown;
    SafeQueue<std::function<void()>> m_queue;
    std::vector<std::thread> m_threads;
    std::mutex m_conditional_mutex;
    std::condition_variable m_conditional_lock;
public:
    ThreadPool(const int n_threads)
        : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {
    }

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;

    ThreadPool & operator=(const ThreadPool &) = delete;
    ThreadPool & operator=(ThreadPool &&) = delete;

    void init();
    void shutdown();
    
    template<typename F, typename...Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;
};
