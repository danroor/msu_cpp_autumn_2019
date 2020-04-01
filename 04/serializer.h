#pragma once
#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream &out;

    template <class T, class... ArgsT>
    Error process(T &&first, ArgsT &&... args) {
        handler(first);
        return process(std::forward<ArgsT>(args)...); //passing remaining arguments to 'process'
    }

    template <class T>
    Error process(T &&last) {
        handler(last);
        return Error::NoError;
    }

    void handler(bool x) {
        out << (x ? "true" : "false") << Separator;
    }

    void handler(uint64_t x) {
        out << x << Separator;
    }

public:
    explicit Serializer(std::ostream &str) : out(str) {}

    template <class T>
    Error save(T &obj) {
        return obj.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
    std::istream &in;

    template <class T, class... ArgsT>
    Error process(T &&first, ArgsT &&... args) {
        if (handler(first) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error process(T &&last) {
        return handler(last);
    }

    Error handler(bool &x) {
        std::string token;
        in >> token;
        if (token == "true") {
            x = true;
        } else if (token == "false") {
            x = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error handler(uint64_t &x) {
        std::string token;
        in >> token;
        try {
            //check for negative number
            if (token[0] < '0' || token[0] > '9') {
                return Error::CorruptedArchive;
            }
            x = std::stoull(token);
            return Error::NoError;
        } catch (const std::exception &exc) {
            return Error::CorruptedArchive;
        }
    }

public:
    explicit Deserializer(std::istream &a) : in(a) {}

    template <class T>
    Error load(T &obj) {
        return obj.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};
