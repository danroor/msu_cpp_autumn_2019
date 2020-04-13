#pragma once
#include <ostream>
#include <iomanip>

class BigInt
{
    using num = unsigned long long;
    using sztype = unsigned int;

    static constexpr num NDIGITS = 18, DIGITS = 1000000000000000000;
    //10^NDIGITS (NDIGITS digits in each group)

    num *number;   //array of groups of digits
    sztype sz;     //size of number array

    bool neg; //True if the number is negative

    int cmp(const BigInt &y) const {
        //-1 if this <  y
        //0  if this == y
        //1  if this >  y
        int sign = 1;

        if (neg) {
            if (y.neg)
                sign = -1;
            else
                return -1;

        } else if (y.neg)
            return 1;

        if (sz < y.sz) return -sign;
        if (sz > y.sz) return sign;

        for (sztype i = 0; i < sz; ++i) {
            if (number[sz - i - 1] < y.number[sz - i - 1]) return -sign;
            if (number[sz - i - 1] > y.number[sz - i - 1]) return sign;
        }

        return 0;
    }

    BigInt add_abs(const BigInt &y) const {  // |this| + |y|
        BigInt res;
        res.neg = neg;
        res.sz = sz > y.sz ? sz : y.sz;
        sztype min = sz < y.sz ? sz : y.sz;

        res.number = new num[res.sz + 1];
        if (res.number == nullptr)
            throw std::bad_alloc();
        num carry = 0;

        for (sztype i = 0; i < min; ++i) {
            res.number[i] = number[i] + y.number[i] + carry;
            carry = res.number[i] / DIGITS;
            res.number[i] %= DIGITS;
        }

        const BigInt *ptr;
        if (min == y.sz)
            ptr = this;
        else
            ptr = &y;

        for (sztype i = min; i < res.sz; ++i) {
            res.number[i] = ptr->number[i] + carry;
            carry = res.number[i] / DIGITS;
            res.number[i] %= DIGITS;
        }

        if (carry != 0) {
            res.number[res.sz++] = carry;
        }
        return res;
    }

    BigInt sub_abs(const BigInt &y) const {  // |this| - |y|, |this| > |y|
        BigInt res;
        res.neg = false;
        res.sz = sz;

        res.number = new num[res.sz];
        if (res.number == nullptr)
            throw std::bad_alloc();
        bool borrow = false;

        for (sztype i = 0; i < y.sz; ++i) {
            if (number[i] < y.number[i] ||  (number[i] == y.number[i] && borrow)) {
                res.number[i] = DIGITS - (borrow ? 1 : 0);
                borrow = true;
            } else {
                borrow = false;
                res.number[i] = 0;
            }
            res.number[i] += number[i] - y.number[i];
            res.number[i] %= DIGITS;
        }

        for (sztype i = y.sz; i < sz; ++i) {
            if (number[i] == 0 && borrow) {
                borrow = true;
                res.number[i] = DIGITS - 1;
            } else {
                res.number[i] = number[i] - (borrow ? 1 : 0);
                borrow = false;
            }
        }

        return res;
    }

public:
    BigInt() : sz(0) {};
    ~BigInt() {
        if (sz > 0) delete [] number;
    }

    BigInt(const int &x) : sz(1) {
        neg = x < 0;
        number = new num[sz];
        long long xx = x;
        number[0] = static_cast <num> (xx >= 0 ? xx : -xx);
    }

    BigInt(const BigInt &x) : sz(x.sz), neg(x.neg) {
        number = new num[sz];
        for (sztype i = 0; i < sz; ++i)
            number[i] = x.number[i];
    }

    BigInt& operator=(const BigInt &x) {
        if (sz > 0)
            delete [] number;

        sz = x.sz;
        neg = x.neg;
        number = new num[sz];
        for (sztype i = 0; i < sz; ++i)
            number[i] = x.number[i];

        return *this;
    }

    BigInt(BigInt &&x) : number(x.number), sz(x.sz), neg(x.neg) { x.sz = 0; }

    BigInt& operator=(BigInt &&x)
    {
        if (sz > 0)
            delete [] number;

        sz = x.sz;
        neg = x.neg;
        number = x.number;
        x.sz = 0;

        return *this;
    }

    friend BigInt operator+(const BigInt &x, const BigInt &y);
    friend BigInt operator-(const BigInt &x, const BigInt &y);

    BigInt operator-() const {
        auto x = BigInt(*this);
        x.neg = !x.neg;
        return x;
    }

    friend bool operator==(const BigInt &x, const BigInt &y);
    friend bool operator!=(const BigInt &x, const BigInt &y);
    friend bool operator> (const BigInt &x, const BigInt &y);
    friend bool operator>=(const BigInt &x, const BigInt &y);
    friend bool operator< (const BigInt &x, const BigInt &y);
    friend bool operator<=(const BigInt &x, const BigInt &y);

    friend std::ostream& operator<<(std::ostream &out, const BigInt &x);
};

std::ostream& operator<<(std::ostream &out, const BigInt &x) {
    if (x.neg)
        out << "-";

    if (x.number[x.sz - 1] != 0 || x.sz == 1)
        out << x.number[x.sz - 1];
    for (BigInt::sztype i = 1; i < x.sz; ++i)
        out << std::setw(BigInt::NDIGITS) << std::setfill('0') << x.number[x.sz - i - 1];

    return out;
}

BigInt operator+(const BigInt&x, const BigInt &y) {
    if (x.neg ^ y.neg) {
        //signs do not match
        BigInt *cpy = new BigInt;
        *cpy = y;

        cpy->neg = !y.neg; //cpy := -y
        int cmpres = x.cmp(*cpy);
        delete cpy; //destroy cpy and free its number array

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
