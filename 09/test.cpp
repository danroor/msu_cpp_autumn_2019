#include <iostream>
#include <fstream>
#include <cstdio>
#include "bin.h"

using namespace std;

int main() {

    std::string in("in.bin"), out("out.bin");

    try {
        bin::generate_random_file(in, 10000000); //10^7
        bin::sort(in, out);

        ifstream f(out, ios::in | ios::binary);
        if (!f)
            throw runtime_error("Error: unable to open file");

        uint64_t prev = 0, cur;
        while (f.read( (char *) &cur, sizeof(cur)  )) {
            if (cur < prev)
                throw runtime_error("Error: numbers are not sorted");
            prev = cur;
        }

        remove(in.c_str());
        remove(out.c_str());

    } catch (const runtime_error &exc) {
        cout << exc.what() << endl;
        return 0;
    }

    cout << "Success!\n";
    return 0;
}
