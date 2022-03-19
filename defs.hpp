#ifndef EMECHS_DEFS_HPP
#define EMECHS_DEFS_HPP

#include <preload.hpp>
#include <string_view>
#include <cctype>
#include <unordered_map>
#include <value.hpp>

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

ValueSharedPtr numericUnopMinus(ValueSharedPtr a);
ValueSharedPtr numericBinopPlus(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopMinus(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopTimes(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopDivide(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopMod(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopQuot(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopRem(ValueSharedPtr a, ValueSharedPtr b);

const ::std::unordered_map<std::string, ::std::function<ValueSharedPtr(ValueSharedPtr)>> UnaryOps {
        {"-", numericUnopMinus},
};

const ::std::unordered_map<std::string, ::std::function<ValueSharedPtr(ValueSharedPtr, ValueSharedPtr)>> BinaryOps {
        {"+", numericBinopPlus},
        {"-", numericBinopMinus},
        {"*", numericBinopTimes},
        {"/", numericBinopDivide},
        {"mod", numericBinopMod},
        {"quotient", numericBinopQuot},
        {"remainder", numericBinopRem},
};

template <typename T>
T abs(T a) {
    return a < 0 ? -a : a;
}

}

#endif //EMECHS_DEFS_HPP
