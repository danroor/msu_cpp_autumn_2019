#ifndef STDLIBS
#define STDLIBS

#include <cstdlib>
#include <iostream>

#endif

#include "matrix.h"

using namespace std;

int main()
{
    int cols = 2, rows1 = 3, cols1 = 4;

    Matrix m(2, 2), m2(rows1, cols1); 

    const Matrix cm(2, cols), cm1(cols, 2); 

    cout << "Matrix 1:\n" << m.getRows() << " rows, " << m.getCols() << " columns\n"
            << m[0][0] << " " << m[0][1] << "\n" << m[1][0] << " " << m[1][1] << "\n\n";

    cout << "Matrix 2:\n" << m2.getRows() << " rows, " << m2.getCols() << " columns\n";
    for (int i = 0; i < m2.getRows(); ++i) {
        for (int j = 0; j < m2.getCols(); ++j)
            cout << m2[i][j] << " ";
        cout << endl;
    }

    cout << "\nConst matrix:\n" << cm.getRows() << " rows, " << cm.getCols() << " columns\n"
            << cm[0][0] << " " << cm[0][1] << "\n" << cm[1][0] << " " << cm[1][1] << "\n\n";

    if (m != m2) cout << "Two non-const matrices successfully compared\n";
    if (m2 != m) cout << "Two non-const matrices successfully compared\n";
    if (m == cm) cout << "Non-const and const matrices successfully compared\n";
    if (cm == m) cout << "Const and non-const matrices successfully compared\n";
    if (cm == cm1) cout << "Two const matrices successfully compared\n";

    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    cout << "Matrix changed:\n" << m[0][0] << " " << m[0][1] << endl << m[1][0] << " "
            << m[1][1] << endl << endl;

    if (m != cm) cout << "Non-const and const matrices successfully compared\n";
    if (cm != m) cout << "Const and non-const matrices successfully compared\n";

    m *= 2;

    cout << "Multiplied by 2:\n" << m[0][0] << " " << m[0][1]
            << endl << m[1][0] << " " << m[1][1] << endl;

    int x = m[1][1];
    cout << "Element of 2nd row, 2nd column: " << x << endl << endl;

    Matrix m1(m);
    if (m == m1) cout << "Copied successfully\n";

    m[0][0] = 0;
    if (m != m1) cout << "Changed successfully\n";

    try {
        m[3000][1] = 2;
    }
    catch (const std::out_of_range &exc) {
        cout << "Exception caught: " << exc.what()  << "\n";
    }
    try {
        m[0][21244] = 2;
    }
    catch (const std::out_of_range &exc) {
        cout << "Exception caught: " << exc.what()  << "\n";
    }
    try {
        m[2][23433] = 2;
    }
    catch (const std::out_of_range &exc) {
        cout << "Exception caught: " << exc.what()  << "\n\n";
    } 

    return 0;
}
