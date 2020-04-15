#include <cassert>
#include <iostream>
#include <stdexcept>

#include "format.h"

using namespace std;

int main()
{
    auto text = format("{1}+{1} = {0}", 2, "one");
    assert(text == "one+one = 2");

    std::string pi("PI");
    text = format("{1} equals to {0}", 3.14159, pi);
    assert(text == "PI equals to 3.14159");
 
    const char *format1 = "{0}{4}{3} + {1}{4}{3} = {2}";
    std::string format2 = "{1} {2} red, {0} {2} blue";

    text = format(format1, "sin", "cos", 1, "(x)", "^2");
    assert(text == "sin^2(x) + cos^2(x) = 1");

    text = format(format2, "violets", "roses", "are");
    assert(text == "roses are red, violets are blue");

    format2 =
"In C l{3}guage, {0} is {3} {1} of false {3}d {2} is {3} {1} of true as well as {3}y other non-zero number";

    text = format(format2, false, "equivalent", true, "an");
    assert(text ==
"In C language, 0 is an equivalent of false and 1 is an equivalent of true as well as any other non-zero number");

    format2 = "An apple a day keeps the doctor away";
    text = format(format2, 1, 2, 3, 4);
    assert(text == "An apple a day keeps the doctor away");

    format2.push_back('{');
    try {
         text = format(format2, 1, 2, 3, 4);
    } catch (std::runtime_error &exc) {
        cout << exc.what() << endl;
    }

    format2 = "{0}}{1}";
    try {
         text = format(format2, 1, 2, 3, 4);
    } catch (std::runtime_error &exc) {
        cout << exc.what() << endl;
    }

    format2 = "{-2} {0} {2}";
    try {
         text = format(format2, 1, 2, 3, 4);
    } catch (std::runtime_error &exc) {
        cout << exc.what() << endl;
    }

    format2 = "{2} {0} {4}";
    try {
         text = format(format2, 1, 2, 3, 4);
    } catch (std::runtime_error &exc) {
        cout << exc.what() << endl;
    }

    format2 = "{2} {0} {}";
    try {
         text = format(format2, 1, 2, 3, 4);
    } catch (std::runtime_error &exc) {
        cout << exc.what() << endl;
    }

    return 0;
}
