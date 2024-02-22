#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool();

private:
    // 用于保存任务的队列
    std::queue<std::function<void()>> tasks;

    // 线程池
    std::vector<std::thread> workers;

    // 用于同步的互斥锁和条件变量
    std::mutex queueMutex;
    std::condition_variable condition;

    bool stop;
};

#endif // __THREADPOOL_H__