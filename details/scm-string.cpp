#include <details/scm-string.ipp>
#include <defs.hpp>
#include <eval.hpp>

namespace emehcs {

ValueP strMakeString(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Char);

    lv::Number a_num {a->get<lv::Number>()};
    lv::Char b_char {b->get<lv::Char>()};
    lv::String ret(a_num, b_char);

    return make_shared_value(ret);
}

ValueP strStringAppend(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);

    return make_shared_value(a->get<lv::String>() + b->get<lv::String>());
}

ValueP strSubstring(ValueP a, ValueP b, ValueP c, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, Number);
    CHECK_TYPE(c, Number);

    return make_shared_value(a->get<lv::String>().substr(b->get<lv::Number>(), b->get<lv::Number>()));
}

}
