#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

using Queue = std::queue<int>;
using Input = std::ifstream;
using Prom  = std::promise<int>;
using Mutex = std::mutex;
using lock_t = std::unique_lock<Mutex>;
using sync_t = std::condition_variable;

void read_file(Input &in, Queue &q1, Queue &q2, Mutex &mut_push, 
               sync_t &sync1, sync_t &sync2, sync_t &sync_push)
{
    bool first = true;
    unsigned long long cur = 0, bufsz = 1000;
    int x, cur_thread = 0;

    lock_t Lock(mut_push, std::defer_lock_t());

    while (in >> x) {
        if (cur == bufsz) {
            cur = 0;
            if (!first) {
                sync_push.wait(Lock);
            }
            else {
                first = false;
            }

            if (cur_thread % 2 == 0) {
                sync1.notify_one();
            }
            else {
                sync2.notify_one();
            }
            cur_thread = (cur_thread + 1) % 2;

        } else {
            cur++;
        }
        if (cur_thread % 2 == 0) {
            q1.push(x);
        }
        else {
            q2.push(x);
        }
    }

    if (first) sync1.notify_one();
    sync_push.wait(Lock);

    q1.push(0);
    q2.push(0);

    sync1.notify_one();
    sync2.notify_one();
}

int sum(Queue &q, Mutex &mut, sync_t &sync, sync_t &sync_push)
{
    bool go = true;
    int x, res = 0;
    while (go) {
        lock_t Lock(mut, std::defer_lock_t());
        sync.wait(Lock, [&q]() { return !q.empty(); });
        while (!q.empty()) {
            x = q.front();
            if (x == 0) {
                go = false;
                break;
            }
            res += x;
            q.pop();
        }
        sync_push.notify_one();
    }

    return res;
}

int file_summary(const std::string &name)
{
    Input in(name, std::ios::in);
    if (!in) throw std::runtime_error("File not opened");
    Queue q1, q2;
    Mutex mut1, mut2,  mut_push;
    sync_t sync1, sync2, sync_push;

    auto task = [](std::function <int (Queue&, Mutex &, sync_t&, sync_t&) > f, Queue &q, 
                   Mutex &mut, sync_t &sync, sync_t &sync_push, Prom &prom) {
        prom.set_value(f(q, mut, sync, sync_push));
    };

    Prom prom1, prom2;

    auto res1 = prom1.get_future();
    auto res2 = prom2.get_future();

    std::thread t1(task, sum, std::ref(q1), std::ref(mut1),
                              std::ref(sync1), std::ref(sync_push), 
                              std::ref(prom1));

    std::thread t2(task, sum, std::ref(q2), std::ref(mut2),
                              std::ref(sync2), std::ref(sync_push), 
                              std::ref(prom2));
    
    read_file(in, std::ref(q1), std::ref(q2), std::ref(mut_push), 
                  std::ref(sync1),std::ref(sync2), std::ref(sync_push));
    t1.join();
    t2.join();

    return res1.get() + res2.get();
}

int main()
{
    std::cout << file_summary("input.txt") << std::endl;

    return 0;
}
