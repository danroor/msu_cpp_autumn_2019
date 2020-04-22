#pragma once
#include <limits>
#include <iostream>
template <class T>
class Allocator
{
    using ptr_t = T*;
    using constref_t = const T&;
public:
    ptr_t alloc(size_t count) {
        ptr_t p = (ptr_t) calloc(count, sizeof(T));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    void dealloc(ptr_t p, size_t count) {
        free(p);
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    void construct(ptr_t p, constref_t val, size_t n) {
        for (size_t i = 0; i < n; i++) {
            new (p + i) T(val);
        }
    }

    void construct(ptr_t p, size_t n) {
        for (size_t i = 0; i < n; i++) {
            new (p + i) T();
        }
    }

    void destroy(ptr_t p, size_t n) {
        for (size_t i = 0; i < n; i++) {
            (p[i]).~T();
        }
    }
};

template <class T>
class Iterator {
    using valptr_t = T*;
    using valref_t = T&;
    using constref_t = const Iterator<T>&;
    
    valptr_t ptr_;
    bool reverse_;
public:
    explicit Iterator(valptr_t ptr, bool reverse) : ptr_(ptr), reverse_(reverse) {}

    bool operator==(constref_t other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(constref_t other) const {
        return ptr_ != other.ptr_;
    }

    valref_t operator*() const {
        return *ptr_;
    }

    Iterator& operator++() {
        if (reverse_) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return *this;
    }

    Iterator operator++(int) {
        Iterator it(ptr_, reverse_);
        if (reverse_) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return it;
    }

    Iterator& operator--() {
        if (reverse_) {
            ++ptr_;
        } else {
            --ptr_;
        }
        return *this;
    }

    Iterator operator--(int) {
        Iterator it(ptr_, reverse_);
        if (reverse_) {
            ++ptr_;
        } else {
            --ptr_;
        }
        return it;
    }

    Iterator& operator+(size_t n) {
        if (reverse_) {
            ptr_ -= n;
        } else {
            ptr_ += n;
        }
        return *this;
    }

    Iterator& operator-(size_t n) {
        if (reverse_) {
            ptr_ += n;
        } else {
            ptr_ -= n;
        }
        return *this;
    }
};

template <class T, class AllocT = Allocator<T> >
class Vector
{
    using ptr_t = T*;
    using ref_t = T&;
    using constref_t = const T&;
    using rvalref_t = T&&;
    using iterator = Iterator<T>;

    size_t sz, cap; //size and capacity
    AllocT alloc_;
    ptr_t data;
public:

    explicit Vector() : sz(0), cap(0), data(nullptr) {}

    explicit Vector(size_t count) : sz(count), cap(count) {
        data = alloc_.alloc(count);
        alloc_.construct(data, count);
    }

    Vector(size_t count, constref_t val) : sz(count), cap(count) {
        data = alloc_.alloc(count);
        alloc_.construct(data, val, count);
    }

    ~Vector() {
        alloc_.destroy(data, sz);
        alloc_.dealloc(data, cap);
    }

    iterator begin() noexcept {
        return iterator(data, false);
    }
    iterator end() noexcept {
        return iterator(data + sz, false);
    }

    iterator rbegin() noexcept {
        return iterator(data + sz - 1, true);
    }
    
    iterator rend() noexcept {
        return iterator(data - 1, true);
    }

    void push_back(rvalref_t value) {
        resize(sz + 1, value); //initialize new element (class T may not have trivial constructor)
        data[sz - 1] = std::move(value);
    }
    void push_back(constref_t value) {
        resize(sz + 1, value);
    }

    void pop_back() {
        if (sz > 0)
            alloc_.destroy(data + --sz, 1);
    }

    void reserve(size_t count) {
        if (count > alloc_.max_size())
            throw std::length_error("Too much elements to reserve");
        if (count <= cap)
            return;

        ptr_t newdata = alloc_.alloc(count);
        for (size_t i = 0; i < sz; ++i)
            alloc_.construct(newdata + i, data[i], 1);
        alloc_.destroy(data, sz);
        alloc_.dealloc(data, cap);
        data = newdata;
        cap = count;
    }

    void resize(size_t newsz) {
        if (newsz == sz) return;
        if (newsz < sz) {
            alloc_.destroy(data + newsz, sz - newsz);
            sz = newsz;
            return;
        }

        if (newsz > cap)
            reserve(newsz > 2 * cap ? newsz : 2 * cap);

        alloc_.construct(data + sz, newsz - sz);
        sz = newsz;
    }

    void resize(size_t newsz, constref_t val) {
        if (newsz == sz) return;
        if (newsz < sz) {
            alloc_.destroy(data + newsz, sz - newsz);
            sz = newsz;
            return;
        }

        if (newsz > cap)
            reserve(newsz > 2 * cap ? newsz : 2 * cap);

        alloc_.construct(data + sz, val, newsz - sz);
        sz = newsz;
    }

    void clear() noexcept {
        alloc_.destroy(data, sz);
        sz = 0;
    }

    ref_t operator[](size_t i) { return data[i]; }
    constref_t operator[](size_t i) const { return data[i]; }

    bool empty() const noexcept { return sz == 0; }
    size_t size() const noexcept { return sz; }
    size_t capacity() const noexcept { return cap; }
};
