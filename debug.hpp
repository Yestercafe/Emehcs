#ifndef EMECHS_DEBUG_HPP
#define EMECHS_DEBUG_HPP

#include <iostream>

namespace emehcs {

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

}

#endif //EMECHS_DEBUG_HPP
