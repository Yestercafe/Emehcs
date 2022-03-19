#ifndef EMECHS_EMEHCS_HPP
#define EMECHS_EMEHCS_HPP

#include <iostream>
#include <memory>

namespace emehcs {

class Value;
class ParserReturns;

using ValueSharedPtr = ::std::shared_ptr<Value>;

template<typename T>
auto make_shared_value(const T& t) {
    return ::std::make_shared<Value>(t);
}

}

#endif //EMECHS_EMEHCS_HPP
