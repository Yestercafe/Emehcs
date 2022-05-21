#include <details/scm-io.ipp>
#include <preload.hpp>
#include <eval.hpp>

#include <iostream>
#include <iomanip>

namespace emehcs {

ValueP lispPrint(ValueP a, EnvironmentP env) {
    auto ans {eval(a, env)};
    ::std::cout << *ans << ' ';
    ::std::flush(::std::cout);
    return ans;
}

ValueP lispPrintLn(ValueP a, EnvironmentP env) {
    auto ans {eval(a, env)};
    ::std::cout << *ans << ::std::endl;
    return ans;
}

ValueP lispPrintFloating(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_INTEGER(b);

    ::std::cout << ::std::fixed << ::std::setprecision(static_cast<::std::int64_t>(b->get<lv::Number>()))
                << a->get<lv::Number>() << ' ';
    ::std::flush(::std::cout);
    return a;
}

ValueP lispPrintFloatingLn(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_INTEGER(b);

    ::std::cout << ::std::fixed << ::std::setprecision(static_cast<::std::int64_t>(b->get<lv::Number>()))
                << a->get<lv::Number>() << ::std::endl;
    return a;
}

}
