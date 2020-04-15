#ifndef BIGINT_H
#define BIGINT_H

#include <ostream>
#include <iomanip>

class BigInt
{
    using num_t = unsigned long long;

    static constexpr num_t NDIGITS = 18, DIGITS = 1000000000000000000;
    //10^NDIGITS (NDIGITS digits in each group)

    num_t *number;   //array of groups of digits
    size_t sz;     //size of number array
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

        for (size_t i = 0; i < sz; ++i) {
            if (number[sz - i - 1] < y.number[sz - i - 1]) return -sign;
            if (number[sz - i - 1] > y.number[sz - i - 1]) return sign;
        }

        return 0;
    }

    BigInt add_abs(const BigInt &y) const {  // |this| + |y|
        BigInt res;
        res.neg = neg;
        res.sz = sz > y.sz ? sz : y.sz;
        size_t min = sz < y.sz ? sz : y.sz;

        res.number = new num_t[res.sz + 1];
        num_t carry = 0;

        for (size_t i = 0; i < min; ++i) {
            res.number[i] = number[i] + y.number[i] + carry;
            carry = res.number[i] / DIGITS;
            res.number[i] %= DIGITS;
        }

        const BigInt *ptr;
        if (min == y.sz)
            ptr = this;
        else
            ptr = &y;

        for (size_t i = min; i < res.sz; ++i) {
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

        res.number = new num_t[res.sz];
        bool borrow = false;

        for (size_t i = 0; i < y.sz; ++i) {
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

        for (size_t i = y.sz; i < sz; ++i) {
            if (number[i] == 0 && borrow) {
                borrow = true;
                res.number[i] = DIGITS - 1;
            } else {
                res.number[i] = number[i] - (borrow ? 1 : 0);
                borrow = false;
            }
        }

        //truncate extra zeros from res.number
        for (size_t i = 0; i < sz - 1 && res.number[sz - i - 1] == 0; ++i, --res.sz);

        return res;
    }

public:
    BigInt() : number(nullptr), sz(0), neg(false) {};
    ~BigInt() {
        if (sz > 0) delete [] number;
    }

    BigInt(int x) : sz(1) {
        neg = x < 0;
        number = new num_t[sz];
        long long xx = x;
        number[0] = static_cast <num_t> (xx >= 0 ? xx : -xx);
    }

    BigInt(const BigInt &x) : sz(x.sz), neg(x.neg) {
        number = new num_t[sz];
        for (size_t i = 0; i < sz; ++i)
            number[i] = x.number[i];
    }

    BigInt& operator=(const BigInt &x) {
        if (sz > 0)
            delete [] number;

        sz = x.sz;
        neg = x.neg;
        number = new num_t[sz];
        for (size_t i = 0; i < sz; ++i)
            number[i] = x.number[i];

        return *this;
    }

    BigInt(BigInt &&x) : number(x.number), sz(x.sz), neg(x.neg) {
        x.sz = 0;
        x.number = nullptr;
    }

    BigInt& operator=(BigInt &&x)
    {
        if (sz > 0)
            delete [] number;

        sz = x.sz;
        neg = x.neg;
        number = x.number;

        x.sz = 0;
        x.number = nullptr;

        return *this;
    }

    friend BigInt operator+(const BigInt &x, const BigInt &y);
    friend BigInt operator-(const BigInt &x, const BigInt &y);

    BigInt operator-() const {
        auto x = BigInt(*this);
        if (x.sz > 1 || x.number[0] != 0) // +0 = -0
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

#endif
