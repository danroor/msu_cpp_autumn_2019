#ifndef STDLIBS
#define STDLIBS

#include <cctype>
#include <cstdlib>
#include <iostream>

#endif

#ifndef PARSER_H
#define PARSER_H

namespace parser {
    using OnNumber = void (*)(long long n);
    using OnString = void (*)(std::string &str);
    using OnBegin = void (*)();
    using OnEnd = void (*)();
}

void reset_on_number_callback();
void reset_on_string_callback();
void reset_on_begin_callback();
void reset_on_end_callback();

void register_on_number_callback(parser::OnNumber callback);
void register_on_string_callback(parser::OnString callback);
void register_on_begin_callback(parser::OnBegin callback);
void register_on_end_callback(parser::OnEnd callback);

void parse(const char* text);

#endif
