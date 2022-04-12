#include <details/scm-io.ipp>
#include <preload.hpp>
#include <eval.hpp>

#include <iostream>

namespace emehcs {

ValueP lispPrintString(ValueP a, EnvironmentP env) {
    auto ans {eval(a, env)};
    ::std::cout << *ans << ::std::endl;
    return ans;
}

}
