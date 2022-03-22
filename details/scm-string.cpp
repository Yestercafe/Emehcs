#include <details/scm-string.ipp>
#include <defs.hpp>
#include <eval.hpp>

namespace emehcs {

ValueSharedPtr strMakeString(ValueSharedPtr a, ValueSharedPtr b) {
    UNPACK_A(Num);
    UNPACK_B(Char);

    lv::Number a_num {a->get<lv::Number>()};
    lv::Char b_char {b->get<lv::Char>()};
    lv::String ret(a_num, b_char);

    return make_shared_value(ret);
}

ValueSharedPtr strStringAppend(ValueSharedPtr a, ValueSharedPtr b) {
    UNPACK_A(Str);
    UNPACK_B(Str);

    return make_shared_value(a->get<lv::String>() + b->get<lv::String>());
}

ValueSharedPtr strSubstring(ValueSharedPtr a, ValueSharedPtr b, ValueSharedPtr c) {
    UNPACK_A(Str);
    UNPACK_B(Num);
    UNPACK(c, Num);

    return make_shared_value(a->get<lv::String>().substr(b->get<lv::Number>(), b->get<lv::Number>()));
}

}
