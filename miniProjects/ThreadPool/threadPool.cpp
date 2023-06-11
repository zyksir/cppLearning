#include <iostream>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <chrono>

#include "SafeQueue.h"
#include "threadPool.h"

void ThreadPool::ThreadWorker::operator()() {
  std::function<void()> func;
  bool dequeued;
  while (!m_pool->m_shutdown) {
    {
      std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
      if (m_pool->m_queue.empty()) {
        // always use a condition variable with a predicate
        m_pool->m_conditional_lock.wait(lock, [this](){ return !this->m_pool->m_queue.empty(); });
      }
      dequeued = m_pool->m_queue.dequeue(func);
    }
    if (dequeued) {
      std::cout << "thread_id " << m_id << "start_to_work" << std::endl;
      func();
    }
  }
}

void ThreadPool::init() {
  for (int i = 0; i < m_threads.size(); ++i) {
    m_threads[i] = std::thread(ThreadWorker(this, i));
  }
}

void ThreadPool::shutdown() {
  m_shutdown = true;
  m_conditional_lock.notify_all();
  
  for (int i = 0; i < m_threads.size(); ++i) {
    if(m_threads[i].joinable()) {
      m_threads[i].join();
    }
  }
}

// Submit a function to be executed asynchronously by the pool
template<typename F, typename...Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
  // Create a function with bounded parameters ready to execute
  std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  // Encapsulate it into a shared ptr in order to be able to copy construct / assign 
  auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

  // Wrap packaged task into void function
  std::function<void()> wrapper_func = [task_ptr]() {
    (*task_ptr)(); 
  };

  // Enqueue generic wrapper function
  m_queue.enqueue(wrapper_func);
  std::cout << "task submitted" << std::endl;

  // Wake up one thread if its waiting
  m_conditional_lock.notify_one();

  // Return future from promise
  return task_ptr->get_future();
}

int main() {
  // Create pool with 2 threads
  ThreadPool pool(2);

  // Initialize pool
  pool.init();
  auto simpleCout = [](const int num, const int sesc) {
    std::cout << num << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sesc));
  };
  std::vector<std::future<void>> simpleFeatures;
  for(int i = 10; i >= 1; --i) {
    simpleFeatures.push_back(pool.submit(simpleCout, i, i));
  }

  for(int i = 10; i >= 1; --i) {
    simpleFeatures[i-1].get();
  }
  
  // Shutdown the pool, releasing all threads
  pool.shutdown();
  return 0;
}