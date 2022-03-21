#ifndef EMEHCS_EVAL_HPP
#define EMEHCS_EVAL_HPP

#include <preload.hpp>
#include <memory>
#include <value.hpp>
#include <parser.hpp>

namespace emehcs {

ValueSharedPtr eval(ValueSharedPtr pValue);

#define UNPACK_A(TYPE)                             \
    do {                                           \
        if ((a = unpack##TYPE(a)) == nullptr)      \
            return nullptr;                        \
    } while (false)

#define UNPACK_B(TYPE)                             \
    do {                                           \
        if ((b = unpack##TYPE(b)) == nullptr)      \
            return nullptr;                        \
    } while (false)

#define UNPACK_AB(TYPE) UNPACK_A(TYPE); UNPACK_B(TYPE)

#define EVAL_A()                                                     \
    do {                                                             \
        a = eval(a);                                                 \
        if (a == nullptr) {                                          \
            ::std::cout << "eval a: can't eval list" << ::std::endl; \
            return nullptr;                                          \
        }                                                            \
    } while (false)

#define EVAL_B()                                                     \
    do {                                                             \
        b = eval(b);                                                 \
        if (b == nullptr) {                                          \
            ::std::cout << "eval b: can't eval list" << ::std::endl; \
            return nullptr;                                          \
        }                                                            \
    } while (false)

#define EVAL_AB() EVAL_A(); EVAL_B()

ValueSharedPtr funcQuote(lv::List& list);
ValueSharedPtr funcIf(lv::List& list);
ValueSharedPtr funcCond(lv::List& list);

const ::std::unordered_map<::std::string, ::std::function<ValueSharedPtr(lv::List&)>> BuiltInFunctor {
        {"quote", funcQuote},
        {"if", funcIf},
        {"cond", funcCond},
};

ValueSharedPtr numericUnopMinus(ValueSharedPtr a);
ValueSharedPtr boolBoolUnopNot(ValueSharedPtr a);
ValueSharedPtr listCar(ValueSharedPtr a);
ValueSharedPtr listCdr(ValueSharedPtr a);
ValueSharedPtr numericBinopPlus(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopMinus(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopTimes(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopDivide(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopMod(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopQuot(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numericBinopRem(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopEq(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopL(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopLe(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopG(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopGe(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr numBoolBinopNeq(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr boolBoolBinopAnd(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr boolBoolBinopOr(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strBoolBinopEq(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strBoolBinopL(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strBoolBinopLe(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strBoolBinopG(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strBoolBinopGe(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr listCons(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr eqv(ValueSharedPtr a, ValueSharedPtr b);

const ::std::unordered_map<std::string, ::std::function<ValueSharedPtr(ValueSharedPtr)>> UnaryOps {
        {"-", numericUnopMinus},
        {"!", boolBoolUnopNot},
        {"car", listCar},
        {"cdr", listCdr},
};

const ::std::unordered_map<std::string, ::std::function<ValueSharedPtr(ValueSharedPtr, ValueSharedPtr)>> BinaryOps {
        {"+", numericBinopPlus},
        {"-", numericBinopMinus},
        {"*", numericBinopTimes},
        {"/", numericBinopDivide},
        {"mod", numericBinopMod},
        {"quotient", numericBinopQuot},
        {"remainder", numericBinopRem},
        {"=", numBoolBinopEq},
        {"<", numBoolBinopL},
        {"<=", numBoolBinopLe},
        {">", numBoolBinopG},
        {">=", numBoolBinopGe},
        {"/=", numBoolBinopNeq},
        {"&&", boolBoolBinopAnd},
        {"||", boolBoolBinopOr},
        {"string=?", strBoolBinopEq},
        {"string<?", strBoolBinopL},
        {"string<=?", strBoolBinopLe},
        {"string>?", strBoolBinopG},
        {"string>=?", strBoolBinopGe},
        {"cons", listCons},
        {"eq?", eqv},
        {"eqv?", eqv},
};

}

#endif //EMEHCS_EVAL_HPP
