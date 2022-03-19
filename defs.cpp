#include <defs.hpp>
#include <algorithm>

namespace emehcs {

ValueSharedPtr numericUnopMinus(ValueSharedPtr a) {
    return make_shared_value(-unpackNum(a)->get<lv::Number>());
}

ValueSharedPtr numericBinopPlus(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() + unpackNum(b)->get<lv::Number>());
}

ValueSharedPtr numericBinopMinus(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() - unpackNum(b)->get<lv::Number>());
}

ValueSharedPtr numericBinopTimes(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() * unpackNum(b)->get<lv::Number>());
}

ValueSharedPtr numericBinopDivide(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() / unpackNum(b)->get<lv::Number>());
}

ValueSharedPtr numericBinopMod(ValueSharedPtr a, ValueSharedPtr b) {
    lv::Number aa = unpackNum(a)->get<lv::Number>(), bb = unpackNum(b)->get<lv::Number>();
    if (bb > 0) {
        return make_shared_value(aa % bb);
    }
    else {
        return make_shared_value((aa * bb < 0 ? -1 : 1) * (-bb - ::emehcs::abs(aa) % ::emehcs::abs(bb)));
    }
}

ValueSharedPtr numericBinopQuot(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() / unpackNum(b)->get<lv::Number>());
}

ValueSharedPtr numericBinopRem(ValueSharedPtr a, ValueSharedPtr b) {
    return make_shared_value(unpackNum(a)->get<lv::Number>() % unpackNum(b)->get<lv::Number>());
}

}
