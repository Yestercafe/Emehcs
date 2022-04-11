#ifndef EMEHCS_EVAL_HPP
#define EMEHCS_EVAL_HPP

#include <preload.hpp>
#include <memory>
#include <value.hpp>
#include <parser.hpp>
#include <exception.hpp>

namespace emehcs {

ValueP eval(ValueP pValue, EnvironmentP env);
ValueP apply(const lv::Function& func, ValueP actual, EnvironmentP super_env);

#define CHECK_TYPE(WHO, TYPE)                                                                           \
    WHO = eval(WHO, env);                                                                               \
    do {                                                                                                \
        if (WHO->get_type() != LispValType::TYPE) {                                                     \
            throw TypeMismatchException("[TypeMismatchException] Should be a `" #TYPE "` but is", WHO); \
        }                                                                                               \
    } while (false)

#define EVAL_A()                                                     \
    do {                                                             \
        a = eval(a, env);                                            \
        if (a == nullptr) {                                          \
            ::std::cout << "eval a: can't eval list" << ::std::endl; \
            return nullptr;                                          \
        }                                                            \
    } while (false)

#define EVAL_B()                                                     \
    do {                                                             \
        b = eval(b, env);                                            \
        if (b == nullptr) {                                          \
            ::std::cout << "eval b: can't eval list" << ::std::endl; \
            return nullptr;                                          \
        }                                                            \
    } while (false)

#define EVAL_AB() EVAL_A(); EVAL_B()

ValueP funcQuote(ValueP pValue, EnvironmentP env);
ValueP funcIf(ValueP pValue, EnvironmentP env);
ValueP funcCond(ValueP pValue, EnvironmentP env);
ValueP funcDefine(ValueP pValue, EnvironmentP env);

const ::std::unordered_map<::std::string, ::std::function<ValueP(ValueP, EnvironmentP)>> BuiltInFunctor{
        {"quote",  funcQuote},
        {"if",     funcIf},
        {"cond",   funcCond},
        {"define", funcDefine},
};

ValueP numericUnopMinus(ValueP a, EnvironmentP env);
ValueP boolBoolUnopNot(ValueP a, EnvironmentP env);
ValueP listCar(ValueP a, EnvironmentP env);
ValueP listCdr(ValueP a, EnvironmentP env);
ValueP numericBinopPlus(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopMinus(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopTimes(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopDivide(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopMod(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopQuot(ValueP a, ValueP b, EnvironmentP env);
ValueP numericBinopRem(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopEq(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopL(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopLe(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopG(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopGe(ValueP a, ValueP b, EnvironmentP env);
ValueP numBoolBinopNeq(ValueP a, ValueP b, EnvironmentP env);
ValueP boolBoolBinopAnd(ValueP a, ValueP b, EnvironmentP env);
ValueP boolBoolBinopOr(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopEq(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopL(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopLe(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopG(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopGe(ValueP a, ValueP b, EnvironmentP env);
ValueP listCons(ValueP a, ValueP b, EnvironmentP env);
ValueP eqv(ValueP a, ValueP b, EnvironmentP env);
ValueP loadFromFile(ValueP a, EnvironmentP env);
ValueP EqAssert(ValueP a, ValueP b, EnvironmentP env);
}

#include <details/scm-string.ipp>

namespace emehcs
{
const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, EnvironmentP)>> UnaryOps {
        {"-0", numericUnopMinus},
        {"not", boolBoolUnopNot},
        {"car", listCar},
        {"cdr", listCdr},
        {"load", loadFromFile},
};

const ::std::unordered_map<::std::string, ::std::function<ValueP(ValueP, ValueP, EnvironmentP)>> BinaryOps {
        {"=", numBoolBinopEq},
        {"<", numBoolBinopL},
        {"<=", numBoolBinopLe},
        {">", numBoolBinopG},
        {">=", numBoolBinopGe},
        {"/=", numBoolBinopNeq},
        {"string=?", strBoolBinopEq},
        {"string<?", strBoolBinopL},
        {"string<=?", strBoolBinopLe},
        {"string>?", strBoolBinopG},
        {"string>=?", strBoolBinopGe},
        {"cons", listCons},
        {"eq?", eqv},
        {"eqv?", eqv},
        {"make-string", strMakeString},
        {"assert", EqAssert},
};

const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, ValueP, EnvironmentP)>> FoldOps {
        {"+", numericBinopPlus},
        {"-", numericBinopMinus},
        {"*", numericBinopTimes},
        {"/", numericBinopDivide},
        {"mod", numericBinopMod},
        {"quotient", numericBinopQuot},
        {"remainder", numericBinopRem},
        {"&&", boolBoolBinopAnd},
        {"||", boolBoolBinopOr},
        {"string-append", strStringAppend}
};

const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, ValueP, ValueP, EnvironmentP)>> TernaryOps {
        {"substring", strSubstring},
};

}

#endif //EMEHCS_EVAL_HPP
