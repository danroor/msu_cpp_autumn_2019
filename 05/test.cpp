#include <cassert>
#include <iostream>

#include "bigint.h"

using namespace std;


int main()
{

    BigInt a(10), b = 10, c, d, e, f = 1;
    d = -3;
    e = 8;
    const BigInt i(5), j(0), k(-5), l(-2);
    c = k;

    assert(a == b);
    assert(a == 10);
    assert(c == k);
    assert(i == -k);

    assert(a != e);
    assert(i != k);
    assert(j != 1);

    assert(a > c);
    assert(a > e);
    assert(d > c);
    assert(j > d);
    assert(e > 0);

    assert(a >= b);
    assert(a >= c);
    assert(c >= k);
    assert(k >= c);
    assert(2 >= -l);

    assert(e < a);
    assert(k < e);
    assert(j < b);
    assert(k < d);
    assert(-4 < d);

    assert(a <= b);
    assert(b <= a);
    assert(i <= b);
    assert(k <= e);
    assert(k <= l);
    assert(-5 <= -i);

    a = b + d + 7;
    cout << b << " + " << d << " + 7 = " << a << endl;
    cout << i << endl;

    a = 2 + e;
    assert(a == b);
    
    cout << k << " + " << l << " = " << k + l << endl;
    cout << k << " + " << a << " = " << k + a << endl;
    cout << a << " + " << k << " = " << a + k << endl;
    cout << f << " + " << l << " = " << f + l << endl;
    cout << l << " + " << f << " = " << l + f << endl;
    cout << k << " + " << i << " = " << k + i << endl;
 
    cout << endl;

    cout << a << " - " << b << " = " << a - b << endl;

    cout << a << " - " << i << " = " << a - i << endl;
    cout << i << " - " << a << " = " << i - a << endl;

    cout << k << " - " << l << " = " << k - l << endl;
    cout << l << " - " << k << " = " << l - k << endl;

    cout << a << " - " << k << " = " << a - k << endl;
    cout << k << " - " << a << " = " << k - a << endl;
    cout << f << " - " << d << " = " << f - d << endl;
    cout << d << " - " << f << " = " << d - f << endl;

    cout << endl;

    a = 2147483647;
    BigInt a10;

    for (int j = 0; j < 10; ++j) {
        b = 0;
        a10 = a + a + a + a + a + a + a + a + a + a; //a * 10
        for (int i = 0; i < 10; ++i)
            b = b + a10;
        a = b; //a multiplied by 100
    }
    //a multiplied by 10^20

    cout << "a = 2147483647\n";
    cout << "a * 10^20 = " << a << endl << endl;

    c = 1;
    for (int i = 0; i < 200; ++i) {
        c = c + c;
    }

    cout << "2^200 = " << c << endl << endl;
    
    cout << "2^200 - 2147483647 * 10^20 = " << endl << " = " << c - a << endl;
    cout << "2147483647 * 10^20 - 2^200 = " << endl << " = " << a - c << endl;

    assert(c - a != c);
    assert(c - a == -(a - c));
    
    a = 1;

    for (int j = 0; j < 15; ++j) {
        b = 0;
        a10 = a + a + a + a + a + a + a + a + a + a; //a * 10
        for (int i = 0; i < 10; ++i)
            b = b + a10;
        a = b; //a multiplied by 100
    }
    //a = 10^30

    cout << "10^30 - 3 = " << a - 3 << endl;
    cout << "10^30 - 2147483647 = " << a - 2147483647 << endl;
    

    return 0;
}
