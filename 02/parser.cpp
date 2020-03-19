#include "parser.h"

namespace parser {
    OnNumber numHandler = nullptr;
    OnString strHandler = nullptr;
    OnBegin beginHandler = nullptr;
    OnEnd endHandler = nullptr;

    bool isnumber(std::string &str)
    {
        size_t sz = str.size(), begin = 0;
        
        if (str[0] == '0' && sz > 1)
            return false;
        if (str[0] == '-') 
            begin = 1;

        for (size_t i = begin; i < sz; ++i)
            if (!std::isdigit(str[i]))
                return false;
        return true;
    }
}

using namespace parser;

void reset_on_number_callback()
{
    numHandler = nullptr;
}

void reset_on_string_callback()
{
    strHandler = nullptr;
}

void reset_on_begin_callback()
{
    beginHandler = nullptr;
}

void reset_on_end_callback()
{
    endHandler = nullptr;
}

void register_on_number_callback(OnNumber callback)
{
    numHandler = callback;
}

void register_on_string_callback(OnString callback)
{
    strHandler = callback;
}

void register_on_begin_callback(OnBegin callback)
{
    beginHandler = callback;
}

void register_on_end_callback(OnEnd callback)
{
    endHandler = callback;
}

void parse(const char* text)
{
    if (beginHandler != nullptr)
        beginHandler();

    size_t sz = 0;
    std::string token;

    do
    {
        if (std::isspace(*text) || !*text) {
            if (sz > 0) {
                if (isnumber(token)) {
                    if (numHandler != nullptr)
                        numHandler(strtoll(token.c_str(), nullptr, 10));
                }
                else if (strHandler != nullptr)
                        strHandler(token);

                token = "";
                sz = 0;
            }
        } else {
            token.push_back(*text);
            sz++;
        }

    } while( *(text++) );

    if (endHandler != nullptr)
        endHandler();
}
