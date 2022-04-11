#ifndef EMECHS_EMEHCS_HPP
#define EMECHS_EMEHCS_HPP

#include <iostream>
#include <memory>
#include <unordered_set>

namespace emehcs {

class Value;
class Environment;

using ValueP = ::std::shared_ptr<Value>;
using EnvironmentP = ::std::shared_ptr<Environment>;

template<typename T>
auto make_shared_value(const T& t) {
    return ::std::make_shared<Value>(t);
}

::std::string show(const Value& value, bool = false);

}

#endif //EMECHS_EMEHCS_HPP
