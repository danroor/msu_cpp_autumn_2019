#include "parser.h"

namespace parser {
    OnNumber numHandler = nullptr;
    OnString strHandler = nullptr;
    OnBegin beginHandler = nullptr;
    OnEnd endHandler = nullptr;

    bool isnumber(const char *str)
    {
        bool res = true;
        if (*str == '0' && *(str + 1) != '\0') return false;
        if (*str == '-') {
            str++;
            if (!*str) res = false;           
        }
        while(*str) {
            if (!std::isdigit(*str)) {
                res = false;
                break;
            }
            str++;
        }
        return res;
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
    if (beginHandler == nullptr) {
        std::cout << "Attention! Parser doesn't have a pre-parsing callback\n";
    } else
        beginHandler();

    unsigned int sz = 0, cnt = 0;
    char *token = nullptr;

    while (*text)
    {
        if (std::isspace(*text) && token != nullptr) {
            if (isnumber(token))
                if (numHandler == nullptr) {
                    std::cout << "Attention! Parser doesn't have a on-number callback\n";
                    std::cout << "The following token has not been handled:\n";
                    std::cout << std::string(token) << std::endl;
                } else
                    numHandler(token);
            else
                if (strHandler == nullptr) {
                    std::cout << "Attention! Parser doesn't have a on-string callback\n";
                    std::cout << "The following token has not been handled:\n";
                    std::cout << std::string(token) << std::endl;
                } else
                    strHandler(token);

            free(token);
            token = nullptr;
            sz = cnt = 0;
        } else {
 
            if (sz == cnt) {
                sz <<= 1;
                sz += 2;
                token = (char *) realloc(token, sz * sizeof(char));
            }

            token[cnt++] = *text;
            token[cnt] = '\0';
        }

        text++;
    }

    if (token != nullptr) {
        if (isnumber(token))
            if (numHandler == nullptr) {
                std::cout << "Attention! Parser doesn't have a on-number callback\n";
                std::cout << "The following token has not been handled:\n";
                std::cout << std::string(token) << std::endl;
            } else
                numHandler(token);
        else
            if (strHandler == nullptr) {
                std::cout << "Attention! Parser doesn't have a on-string callback\n";
                std::cout << "The following token has not been handled:\n";
                std::cout << std::string(token) << std::endl;
            } else
                strHandler(token);

        free(token);
    }

    if (endHandler == nullptr) {
        std::cout << "Attention! Parser doesn't have a post-parsing callback\n";
    } else
        endHandler();
}
