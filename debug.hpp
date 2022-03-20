#ifndef EMECHS_DEBUG_HPP
#define EMECHS_DEBUG_HPP

#include <iostream>

#ifdef __EMEHCS_DEBUG
#define LOG ::std::cout
#else
#define LOG ::std::cerr
#endif

#endif //EMECHS_DEBUG_HPP
