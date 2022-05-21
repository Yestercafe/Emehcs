#include <defs.hpp>
#include <algorithm>
#include <eval.hpp>
#include <debug.hpp>
#include <exception.hpp>
#include <cmath>

namespace emehcs {

bool isSymbol(lv::Char c) {
    return ::std::any_of(Symbol.cbegin(), Symbol.cend(), [c](const char& s) { return s == c; });
}

bool isODigit(lv::Char c) {
    return c >= '0' && c <= '7';
}

bool isXDigit(lv::Char c) {
    return ::std::isxdigit(c);
}

bool isInteger(lv::Number n) {
    return ::std::floor(n) == ::std::ceil(n);
}

}
