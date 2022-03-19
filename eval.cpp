#include <eval.hpp>
#include <debug.hpp>

namespace emehcs {

template<typename NumericBinop>
static ValueSharedPtr fold_aux(NumericBinop op, ValueSharedPtr list, ValueSharedPtr res, size_t p) {
    if (p >= list->get<lv::List>().size()) {
        return res;
    }
    res = op(res, list->get<lv::List>()[p]);
    return fold_aux(op, list, res, p + 1);
}

template<typename NumericBinop>
static ValueSharedPtr fold(NumericBinop op, ValueSharedPtr list) {
    ValueSharedPtr res = op(list->get<lv::List>()[1], list->get<lv::List>()[2]);
    return fold_aux(op, list, res, 3);
}

ValueSharedPtr eval(ValueSharedPtr pValue) {
    switch (pValue->get_type()) {
        case LispValType::String:
        case LispValType::Number:
        case LispValType::Bool:
            // return pValue;
            break;
        case LispValType::List:
            if (pValue->get<lv::List>()[0]->get_type() == LispValType::Atom) {
                if (pValue->get<lv::List>()[0]->get<lv::Atom>().str == "quote") {
                    return pValue->get<lv::List>()[1];
                }

                auto func {pValue->get<lv::List>()[0]->get<lv::Atom>()};

                if (pValue->get<lv::List>().size() == 2) {
                    auto fnd = UnaryOps.find(func.str);
                    if (fnd != UnaryOps.cend()) {
                        return fnd->second(pValue->get<lv::List>()[1]);
                    }
                }
                auto fnd {BinaryOps.find(func.str)};
                if (fnd != BinaryOps.cend()) {
                    return fold(fnd->second, pValue);
                }
            }
        default:
            break;
    }

    return pValue;
}

}
