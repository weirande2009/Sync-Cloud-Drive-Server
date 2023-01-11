#include "simple_server/thread_pool.h"

ThreadPool::ThreadPool(unsigned int size) {
    for (unsigned int i = 0; i < size; ++i) {
        workers.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition_variable.wait(lock, [this]() { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition_variable.notify_all();
    for (std::thread &th : workers) {
        if (th.joinable()) {
            th.join();
        }
    }
}
