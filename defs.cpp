#include <defs.hpp>
#include <algorithm>
#include <eval.hpp>
#include <debug.hpp>
#include <exception.hpp>

namespace emehcs {

bool isSymbol(char c) {
    return ::std::any_of(Symbol.cbegin(), Symbol.cend(), [c](const char& s) { return s == c; });
}

bool isODigit(char c) {
    return c >= '0' && c <= '7';
}

bool isXDigit(char c) {
    return ::std::isxdigit(c);
}

}
