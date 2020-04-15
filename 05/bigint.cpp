#include "bigint.h"

std::ostream& operator<<(std::ostream &out, const BigInt &x) {
    if (x.neg)
        out << "-";

    if (x.number[x.sz - 1] != 0 || x.sz == 1)
        out << x.number[x.sz - 1];
    for (size_t i = 1; i < x.sz; ++i)
        out << std::setw(BigInt::NDIGITS) << std::setfill('0') << x.number[x.sz - i - 1];

    return out;
}

BigInt operator+(const BigInt&x, const BigInt &y) {
    if (x.neg ^ y.neg) {
        //signs do not match
        int cmpres = x.cmp(-y);

        if (y.neg) {
            //y < 0, x >= 0
            switch(cmpres) {
            case -1:
                //x < -y, |x| < |y|
                return -y.sub_abs(x);

            case 1:
                //x > -y, |x| > |y|
                return x.sub_abs(y);

            default: //case 0:
                return BigInt(0); //this = -y
            }
        } else {
            // y >= 0, x < 0
            switch(cmpres) {
            case -1:
                //x < -y, |x| > |y|

                return -x.sub_abs(y);

            case 1:
                //x > -y, |x| < |y|
                return y.sub_abs(x);
            default: //case 0:
                return BigInt(0); //x = -y
            }
        }
    } else
        return x.add_abs(y);
}

BigInt operator-(const BigInt &x, const BigInt &y) {
    if (x.neg ^ y.neg) {
        return x.add_abs(y);
    }
    else {
        //signs do match
        if (y.neg) {
            // y < 0, x < 0
            switch(x.cmp(y)) {
            case -1:
                //x < y, |x| > |y|
                return -x.sub_abs(y);

            case 1:
                //x > y, |x| < |y|
                return y.sub_abs(x);

            default: //case 0:
                return BigInt(0); //x = y
            }
        } else {
            //y >= 0, x >= 0
            switch(x.cmp(y)) {
            case -1:
                //x < y, |x| < |y|
                return -y.sub_abs(x);

            case 1:
                //x > y, |x| > |y|
                return x.sub_abs(y);

            default: //case 0:
                return BigInt(0); //x = y
            }
        }
   }
}

bool operator==(const BigInt &x, const BigInt &y) {
    return x.cmp(y) == 0;
}
bool operator!=(const BigInt &x, const BigInt &y) {
    return x.cmp(y) != 0;
}

bool operator>(const BigInt &x, const BigInt &y) {
    return x.cmp(y) == 1;
}

bool operator>=(const BigInt &x, const BigInt &y) {
    return x.cmp(y) >= 0;
}

bool operator<(const BigInt &x, const BigInt &y) {
    return x.cmp(y) == -1;
}

bool operator<=(const BigInt &x, const BigInt &y) {
    return x.cmp(y) <= 0;
}
