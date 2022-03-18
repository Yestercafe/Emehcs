#ifndef EMECHS_DEFS_HPP
#define EMECHS_DEFS_HPP

#include <preload.hpp>
#include <string_view>
#include <cctype>
#include <unordered_map>

namespace emehcs {

using uint = ::std::size_t;

constexpr ::std::string_view Symbol = "!$%&|*+-/:<=?>@^_~#";

constexpr bool issymbol(char c) {
    return ::std::any_of(Symbol.cbegin(), Symbol.cend(), [c](const char& s) { return s == c; });
}

constexpr bool isodigit(char c) {
    return c >= '0' && c <= '7';
}

constexpr bool isxdigit(char c) {
    return ::std::isxdigit(c);
}

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

}

#endif //EMECHS_DEFS_HPP
