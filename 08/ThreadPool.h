#pragma once
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>

class ThreadPool
{
    static constexpr size_t DEFAULT_SIZE = 10;

    using task_t = std::function<void()>;

    std::condition_variable sync;
    std::vector <std::thread> pool;
    std::queue <task_t> task_queue;
    std::mutex queue_access;
    std::atomic<bool> alive;
    size_t sz;

    //non-void functions
    template <class Func, class Promise, class... Args>
    void execTask(Func func, Promise prom, Args... args) {
        prom->set_value(func(args...));
        delete prom;
    }

    //void functions
    template <class Func, class... Args>
    void execTask(Func func, std::promise<void> *prom, Args... args) {
        prom->set_value();
        delete prom;
        func(args...);
    }

public:
    explicit ThreadPool(size_t poolSize = DEFAULT_SIZE) : alive(true), sz(poolSize) {
        auto doTasks = [this]() {
            while (alive) {
                std::unique_lock <std::mutex> Lock(queue_access);
                if (task_queue.empty()) {
                    sync.wait(Lock);
                    continue;
                }
                task_t task(std::move(task_queue.front()));
                task_queue.pop();
                Lock.unlock();
                task();
            }
            
        }; 
        for (size_t i = 0; i < sz; ++i) {
            pool.emplace_back(doTasks);
        }
    }
    ~ThreadPool() {
        join();
    }

    void join() {
        if (!alive)
            return;
        alive = false;
        sync.notify_all();

        for (size_t i = 0; i < sz; ++i) {
            pool[i].join(); 
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {

        using T = std::promise< decltype(func(args...)) >;

        auto prom = new T;
        auto future = prom->get_future();

        auto task = [this](Func func, T *prom, Args... args) {
            execTask(func, prom, args...);
        };

        queue_access.lock();
        task_queue.push(std::bind(task, func, prom, args...));
        queue_access.unlock();

        sync.notify_one();
        return future;
    }

};
