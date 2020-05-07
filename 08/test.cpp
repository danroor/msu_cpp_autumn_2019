#include <iostream>
#include <future>
#include <cassert>
#include "ThreadPool.h"

using namespace std;

int f1(int a, int b, int c) { return a + b + c; }
void code(char c, int *x) { *x = int(c); }
void incr(int *a) { (*a)++; }

//concatenate digits to string
void dig_to_str(std::string &s, int x1, int x2, int x3, int x4) {
    char c1 = x1 > -1 && x1 < 10 ? char(x1 + 48) : '#';
    char c2 = x2 > -1 && x2 < 10 ? char(x2 + 48) : '#';
    char c3 = x3 > -1 && x3 < 10 ? char(x3 + 48) : '#';
    char c4 = x4 > -1 && x4 < 10 ? char(x4 + 48) : '#';

    s.push_back(c1);
    s.push_back(c2);
    s.push_back(c3);
    s.push_back(c4);
}

int main()
{
    int y = 1, t = 0;

    ThreadPool pool1(8), pool2;

    auto task2 = pool1.exec(code, 'A', &y);
    auto task1 = pool1.exec(f1, 1, 2, 3);
    
    int x = task1.get();
    
    std::string s;

    auto task3 = pool2.exec(dig_to_str, std::ref(s), 1, 2, 42, 5);
    pool1.join();
    auto task4 = pool2.exec(f1, 3, 3, 3);

    decltype(task1) tasks[100];

    ThreadPool pool3(15);

    auto task5 = pool3.exec(incr, &t);
    task5.get();
    decltype(task5) tasks2[100];


    for (size_t i = 0; i < 100; ++i) {
        tasks[i] = pool3.exec(f1, i, i, i);
        tasks2[i] = pool3.exec(incr, &t);
    }

    task2.get();
    task3.get();

    for (size_t i = 0; i < 100; ++i) {
        assert(tasks[i].get() == 3 * i);
        tasks2[i].get();
    }
    assert(t == 101);

    cout << s << endl << y << endl << task4.get() << endl;    

    return 0;
}
