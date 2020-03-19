#ifndef STDLIBS
#define STDLIBS

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

#endif

#include <cmath>

#include "parser.h"

using namespace std;

void begin1() {
    cout << "Parsing started\n";
}

void begin2() {
    cout << "Getting ready for text parsing...\n";
}

void begin3() {
    cout << "Let's go!\n";
}

void end1() {
    cout << "Parsing finished\n";
}

void end2() {
    cout << "Parsing complete\n";
}

void end3() {
    cout << "Bye!\n";
}

void num1(long long n) {
    if (n < 0)
        cout << n << " is negative" << endl;
    else
        cout << "sqrt(" << n << ") = " << sqrt(n) << endl;
}

void num2(long long n) {
    cout << n << " + 1 = " << n + 1 << endl;
}

char *arr = nullptr;

void num3(long long n) {
    if (n <= 0) {
        cout << "Positive number expected, non-positive received\n";   
        return;
    } 

    arr = (char *) malloc((n + 1) * sizeof(char));
    if (arr == nullptr) {
        cout << "Not enough memory for your array :(\n";
        return;
    }
    for (int i = 0; i < n; ++i) {
        arr[i] = 'a';
    }

    arr[n] = '\0';
    cout << "Here is your string:\n" << string(arr) << endl;
}

void str1(std::string &str) {
    cout << str << ":\nThis token is not a number\n";
}

void str2(std::string &str) {
    size_t sz = str.size();
    for (size_t i = 0; i < sz; ++i)
        if (!isalpha(str[i])) {
            cout << str << ":\nThis token is not an English word\n";
            return;
        }

    cout << str << ":\nThis token is an English word\n";
    return;
}

void str3(std::string &str) {
    cout << str << ":\nI want numbers, not strings!\n";
}

int main()
{
    const char *text1 = "	Deploying   torpedo in 3  2 1";
    const char *text2 = " 	 Qwerty \n 123 -78 -qwe --89 0 056 компиляция, compilation complete.";
    const char *text3 = "Please,     give   me  a \n\n string   of   10   characters  ";
    const char *text4 = "An  apple  a day  keeps  the  \ndoctor  away 12";
    const char *text5 = "2 + 2 * 2 = 6";

    register_on_begin_callback(begin2);
    register_on_end_callback(end2);
    register_on_number_callback(num2);

    cout << "Text:\n" << string(text4) << "\n\n";
    parse(text4);

    reset_on_number_callback();
    register_on_string_callback(str1);

    cout << "Text:\n" << string(text5) << "\n\n";
    parse(text5);

    reset_on_begin_callback();
    register_on_end_callback(end1);
    register_on_number_callback(num1);
    register_on_string_callback(str1);
    
    cout << "Text:\n" << string(text1) << "\n\n";
    parse(text1);

    cout << "\n\n";

    reset_on_end_callback();    
    
    register_on_begin_callback(begin1);
    register_on_number_callback(num2);
    register_on_string_callback(str2);

    cout << "Text:\n" << string(text2) << "\n\n";
    parse(text2);

    cout << "\n\n";

    register_on_begin_callback(begin3);
    register_on_end_callback(end3);
    register_on_number_callback(num3);
    register_on_string_callback(str3);

    cout << "Text:\n" << string(text3) << "\n\n";
    parse(text3);

    return 0;
}
