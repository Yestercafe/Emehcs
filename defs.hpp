#ifndef EMECHS_DEFS_HPP
#define EMECHS_DEFS_HPP

#include <preload.hpp>
#include <string_view>
#include <cctype>
#include <unordered_map>
#include <value.hpp>

namespace emehcs {

using size_t = ::std::size_t;

constexpr ::std::string_view Symbol = "!$%&|*+-/:<=?>@^_~#";

bool isSymbol(char c);
bool isODigit(char c);
bool isXDigit(char c);
bool isInteger(lv::Number n);

const ::std::unordered_map<char, char> EscapedMap {
        {'b', '\b'},
        {'n', '\n'},
        {'f', '\f'},
        {'r', '\r'},
        {'t', '\t'},
        {'\\', '\\'},
        {'\"', '\"'},
        {'\'', '\''},
};

template <typename T>
T abs(T a) {
    return a < 0 ? -a : a;
}

}

#endif //EMECHS_DEFS_HPP
