#pragma once
#include <sstream>
#include <vector>
#include <cctype>
#include <stdexcept>

class Format
{
    size_t counter;
    std::vector<std::string> format_args;

    template <class T>
    void save_args(T&& last)
    {
        std::stringstream strm;
        std::string buf;
        strm << last;
        strm >> buf;
        format_args.push_back(buf);
        counter++;
    }

    template <class T, class... ArgsT>
    void save_args(T&& first, ArgsT&&... args)
    { 
        std::stringstream strm;
        std::string buf;
        strm << first;
        strm >> buf;
        format_args.push_back(buf);
        counter++;
        save_args(std::forward<ArgsT>(args)...);
    }

public:
    template <class... ArgsT>
    Format(ArgsT&&... args) : counter(0) {
        save_args(std::forward<ArgsT>(args)...);
    }

    const std::string& operator[](size_t i) const {
        return format_args[i];
    }

    size_t get_counter() const { return counter; }
};

template <class... ArgsT>
std::string format(const std::string &str, ArgsT&&... args) { 
    Format format_args(std::forward<ArgsT>(args)...);
    std::stringstream res;

    size_t num, len = str.length(), 
                max_arg = format_args.get_counter();
    
    for (size_t i = 0; i < len; ++i) {
        if (str[i] == '{') {
            if (++i == len || str[i] == '}') {
                throw std::runtime_error("Expected format argument number");
            }
            num = 0;
            while (str[i] != '}') {
                if (!isdigit(str[i])) {
                    throw std::runtime_error("Invalid character in the scope");
                }
                num *= 10;
                num += str[i] - '0';
                i++;
            }
            if (num >= max_arg) {
                throw std::runtime_error("Invalid format argument number");
            }
            res << format_args[num];
        } else if (str[i] == '}') {
            throw std::runtime_error("Expected opening scope before closing scope");
        } else
            res << str[i];
    }

    return res.str();
}
