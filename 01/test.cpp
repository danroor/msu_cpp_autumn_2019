#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>

#include "allocator.h"

using namespace std;

int main()
{ 
    makeAllocator(100);
    
    auto begin = chrono::high_resolution_clock::now();
    char *str = alloc(20);
    auto end = chrono::high_resolution_clock::now();

    if (str == nullptr) cout << "Error: memory not allocated" << endl;
    str[0] = 'a';
    str[1] = 'b';
    str[2] = 'c';
    str[3] = '\0';
    if (strcmp(str, "abc") != 0) 
        cout << "Error: allocated memory working incorrectly" << endl;
    cout << string(str) << endl;

    long long time1 = chrono::duration_cast<chrono::microseconds> (end - begin).count();

    begin = chrono::high_resolution_clock::now();
    char *b = (char *) malloc(20);
    end = chrono::high_resolution_clock::now();
    free(b);
    long long time2 = chrono::duration_cast<chrono::microseconds> (end - begin).count();

    if (time1 > time2) cout << "Error: allocator is slower than malloc" << endl;
    
    cout << "Array of integer numbers:" << endl;
    int *a = (int *) alloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) 
        a[i] = i * i;
    for (int i = 0; i < 10; ++i)
        cout << a[i] << ' ';

    cout << endl;

    char *str1 = alloc(100);
    if (str1 != nullptr)
        cout << "Error: memory allocation after allocator overflow" << endl;

    reset();

    double *dbl = (double *) alloc(sizeof(double) * 8);
    if ((char *) dbl != str)
        cout << "Error: reset working incorrectly" << endl;

    for (int i = 0; i < 8; ++i) 
        dbl[i] = sin(M_PI * i / 6);

    cout << "Array of floating point numbers (sinus values):" << endl;
    for (int i = 0; i < 8; ++i)
        cout << "sin(" << i << "*pi / 6) = " << dbl[i] << endl;

    free(str);
    return 0;
}
