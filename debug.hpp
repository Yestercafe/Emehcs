#ifndef EMECHS_DEBUG_HPP
#define EMECHS_DEBUG_HPP

#include <iostream>

#ifndef __EMEHCS_DEBUG
#define LOG()
#else
void LOG(::std::ostream& os) {
    os << ::std::endl;
}

template<typename Arg, typename ...Args>
void LOG(::std::ostream& os, const Arg& arg, const Args& ...args) {
    os << arg << ' ';
    LOG(os, args...);
}
#endif

#define LOG_STDOUT(...) LOG(::std::cout, __VA_ARGS__)
#define LOG_STDERR(...) LOG(::std::cerr, __VA_ARGS__)

#endif //EMECHS_DEBUG_HPP
