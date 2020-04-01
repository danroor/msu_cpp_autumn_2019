#pragma once
#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream& out;

    template <class T, class... ArgsT>
    Error process(T &&first, ArgsT &&... args) {
        handler(first);
        return process(std::forward<ArgsT>(args)...);
    }

    template <class Last>
    Error process(Last &&last) {
        handler(last);
        return Error::NoError;
    }

    void handler(bool a) {
        out << (a ? "true" : "false") << Separator;
    }

    void handler(uint64_t a) {
        out << a << Separator;
    }

public:
    explicit Serializer(std::ostream &a) : out(a) {}

    template <class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
    std::istream& in;

    template <class T, class... ArgsT>
    Error process(T &&first, ArgsT &&... args) {
        if (handler(first) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    template <class Last>
    Error process(Last &&last) {
        return handler(last);
    }

    Error handler(bool &a) {
        std::string token;
        in >> token;
        if (token == "true") {
            a = true;
        } else if (token == "false") {
            a = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error handler(uint64_t &a) {
        std::string token;
        in >> token;
        try {
            if (token[0] < '0' || token[0] > '9') {
                return Error::CorruptedArchive;
            }
            a = std::stoull(token);
            return Error::NoError;
        } catch (const std::exception &e) {
            return Error::CorruptedArchive;
        }
    }

public:
    explicit Deserializer(std::istream &a) : in(a) {}

    template <class T>
    Error load(T &object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};
