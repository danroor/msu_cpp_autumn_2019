#include <cassert>
#include <iostream>

#include "Vector.h"

using namespace std;

class qq
{
    int x;    
public:
    qq() = delete;
    qq(int xx) : x(xx) {}
    qq(const qq &qobj) : x(qobj.get()) {}
    int get() const { return x; }
};

int main()
{
    Vector<int> vi1;
    Vector<int> vi2(5);
    Vector<string> vs(10, "shazam");

    assert(vi1.size() == 0 && vi1.capacity() == 0);
    assert(vi2.size() == 5 && vi2.capacity() == 5);
    assert(vs.size() == 10 && vs.capacity() == 10);

    assert(vi1.empty());
    assert(!vi2.empty());

    vi1.pop_back();
    vi2.pop_back();
    assert(vi2.size() == 4);

    vs[0] = "badum";
    cout << vs[0] << endl << vs[1] << endl;
    
    vs.reserve(11);
    vs.push_back("123");
    vs.push_back("456");
    assert(vs.size() == 12);

    int x = 1;
    vi1.push_back(x);
    vi1.push_back(move(x));
    assert(vi1.size() == 2);

    vi2.resize(2);
    for (int i = 0; i < 2; ++i)
        cout << vi2[i] << " ";
    cout << endl;

    vi2.resize(4);
    for (int i = 0; i < 4; ++i)
        cout << vi2[i] << " ";
    cout << endl;

    vi2.resize(7, 7);
    for (int i = 0; i < 7; ++i)
        cout << vi2[i] << " ";
    cout << endl;
    assert(vi2.capacity() == 10); // capacity == 2 
                                  //7 < 5 * 2 -> capacity *= 2

    vs.reserve(30);
    assert(vs.capacity() >= 30);

    for (auto it = vi2.begin(); it != vi2.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    assert(*(vi2.begin() + 2) == 0);
    assert(*(vi2.end() - 2) == 7);
    auto it1 = vi2.end();
    --it1;
    it1--;
    assert(*it1 == 7);

    for (auto it = vi2.rbegin(); it != vi2.rend(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    assert(*(vi2.rbegin() + 2) == 7);
    assert(*(vi2.rend() - 2) == 0);
 
    try {
        vs.reserve(89237489723894923ull);
    } catch (...) { cout << "Exception caught" << endl; }    

    vs.clear();
    assert(vs.size() == 0 && vs.begin() == vs.end());
 
    Vector<qq> qv(10, qq(10));
    assert(qv[2].get() == 10);
    qv.reserve(100);
    assert(qv.size() == 10 && qv.capacity() >= 100 && qv[2].get() == 10);
    qv.push_back(qq(10));
    qv.push_back(qq(10));
    qv.push_back(qq(10));
    qv.push_back(qq(10));

    return 0;
}
