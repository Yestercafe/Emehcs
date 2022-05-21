#include <eval.hpp>
#include <debug.hpp>
#include <exception.hpp>
#include <defs.hpp>
#include <environment.hpp>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stack>
#include <preload.hpp>
#include <string_view>
#include <sstream>

namespace emehcs {

template<typename NumericBinop>
static ValueP fold_aux(NumericBinop op, ValueP list, ValueP res, size_t p, EnvironmentP env) {
    if (p >= list->get<lv::List>().size()) {
        return res;
    }
    res = op(res, list->get<lv::List>()[p], env);
    return fold_aux(op, list, res, p + 1, env);
}

template<typename NumericBinop>
static ValueP fold(NumericBinop op, ValueP list, EnvironmentP env) {
    ValueP res = op(list->get<lv::List>()[1], list->get<lv::List>()[2], env);
    return fold_aux(op, list, res, 3, env);
}

ValueP eval(ValueP pValue, EnvironmentP env) {
    switch (pValue->get_type()) {
        case LispValType::String:
        case LispValType::Number:
        case LispValType::Bool:
            return pValue;
        case LispValType::List: {
            if (pValue->get<lv::List>().size() > 0 && pValue->get<lv::List>()[0]->get_type() == LispValType::Atom) {
                auto list {pValue->get<lv::List>()};
                auto func {list[0]->get<lv::Atom>()};
                if (auto fnd {BuiltInFunctor.find(func.str)}; fnd != BuiltInFunctor.cend()) {
                    return fnd->second(pValue, env);
                }

                // lambda expression
                if (func.str == "lambda") {
                    if (list.size() < 3) {
                        throw BadSpecialFormException("[BadSpecialFormException] Unexpected `lambda` syntax, should consist of (lambda params body...), but it is", pValue);
                    }

                    if (list[1]->get_type() != LispValType::List) {
                        throw BadSpecialFormException("[BadSpecialFormException] Unexpected `lambda` syntax, should be argument list, but it is", list[1]);
                    }

                    ::std::deque<::std::string> params(1, "lambda");   // anonymous function named as "lambda"
                    for (auto&& param : list[1]->get<lv::List>()) {
                        if (param->get_type() != LispValType::Atom) {
                            throw BadSpecialFormException("[BadSpecialFormException] Unexpected `lambda` syntax, an argument should an `Atom`, but it is", param);
                        }
                        params.push_back(param->get<lv::Atom>().str);
                    }

                    ::std::deque<ValueP> body;
                    const auto I {list.size()};
                    for (size_t i {2}; i < I; ++i) {
                        body.push_back(list[i]);
                    }

                    return make_shared_value(lv::Function(params, body));
                }

                {
                    auto fnd = ZeroOps.find(func.str);
                    if (fnd != ZeroOps.cend()) {
                        if (list.size() != 1) {
                            throw NumArgsException(0, pValue);
                        }
                        return fnd->second(env);
                    }
                }
                {
                    auto fnd = UnaryOps.find(func.str);
                    if (fnd != UnaryOps.cend()) {
                        if (list.size() != 2) {
                            throw NumArgsException(1, pValue);
                        }
                        return fnd->second(list[1], env);
                    }
                }
                {
                    auto fnd = BinaryOps.find(func.str);
                    if (fnd != BinaryOps.cend()) {
                        if (list.size() != 3) {
                            throw NumArgsException(2, pValue);
                        }
                        return fnd->second(list[1], list[2], env);
                    }
                }
                {
                    auto fnd = TernaryOps.find(func.str);
                    if (fnd != TernaryOps.cend()) {
                        if (list.size() != 4) {
                            throw NumArgsException(3, pValue);
                        }
                        return fnd->second(list[1], list[2], list[3], env);
                    }
                }
                {
                    auto fnd{FoldOps.find(func.str)};
                    if (fnd != FoldOps.cend()) {
                        if (list.size() < 3) {
                            throw NumArgsException(2, pValue, true);
                        }
                        return fold(fnd->second, pValue, env);
                    }
                }
                {
                    auto functor {env->get(func.str)};
                    if (functor) {
                        switch (functor->get_type()) {
                            case LispValType::Function: {
                                auto& function{functor->get<lv::Function>()};
                                const auto param_list_len{function.params.size()};
                                if (list.size() != param_list_len) {
                                    throw NumArgsException(param_list_len - 1, pValue);
                                }
                                return apply(function, pValue, env);
                            }
                            case LispValType::Atom: {
                                pValue->get<lv::List>().front() = env->get(func.str);
                                return eval(pValue, env);
                            }
                            default:
                                break;
                        }
                    }
                }
            }
            if (pValue->get<lv::List>().size() > 0 && pValue->get<lv::List>()[0]->get_type() == LispValType::List) {
                pValue->get<lv::List>()[0] = eval(pValue->get<lv::List>()[0], env);
                return eval(pValue, env);
            }
            if (pValue->get<lv::List>().size() > 0) {
                if (pValue->get<lv::List>()[0]->get_type() == LispValType::Function) {
                    auto list {pValue->get<lv::List>()};
                    auto& functor {pValue->get<lv::List>()[0]};
                    auto& function{functor->get<lv::Function>()};
                    const auto param_list_len{function.params.size()};
                    if (list.size() != param_list_len) {
                        throw NumArgsException(param_list_len - 1, pValue);
                    }
                    return apply(function, pValue, env);
                }
                throw NotFunctionException("[NotFunctionException] Head of a `List` is not a function", pValue->get<lv::List>()[0]);
            }
            else {
                throw BadSpecialFormException("[BadSpecialFormException] A empty `List` without quote", pValue);
            }
        }
        case LispValType::Atom: {
            if (Keywords.contains(pValue->get<lv::Atom>().str)) {
                return pValue;
            }
            auto var = env->get(pValue->get<lv::Atom>().str);
            if (var) {
                return var;
            }
            else {
                throw IdentifierException("[IdentifierException] Unknown identifier", pValue);
            }
        }
        case LispValType::Char:
            return pValue;
        default:
            break;
    }

    throw BadSpecialFormException("[BadSpecialFormException] Unrecognized special form", pValue);
}

ValueP apply(const lv::Function& func, ValueP actual, EnvironmentP super_env) {
    EnvironmentP local_env;
    if (func.closure) {
        local_env = ::std::make_shared<Environment>(super_env, func.closure);
    }
    else {
        local_env = ::std::make_shared<Environment>(super_env);
    }

    auto actual_param_list {actual->get<lv::List>()};
    for (size_t i {1}; i < actual_param_list.size(); ++i) {
        local_env->put(func.params[i], eval(actual_param_list[i], super_env));
    }

    for (size_t i {}; i < func.body.size() - 1; ++i) {
        eval(func.body[i], local_env);
    }
    auto ret {eval(func.body.back(), local_env)};
    if (ret->get_type() == LispValType::Function) {  // if return a function should bind closure
        ret->get<lv::Function>().closure = local_env;
    }
    return ret;
}

ValueP funcQuote(ValueP pValue, EnvironmentP env) {
    return pValue->get<lv::List>()[1];
}

ValueP funcIf(ValueP pValue, EnvironmentP env) {
    auto list {pValue->get<lv::List>()};
    // (if pred conseq alt)
    if (list.size() == 4) {
        auto pred {eval(list[1], env)};
        auto conseq {list[2]};
        auto alt {list[3]};
        CHECK_TYPE(pred, Bool);
        if (pred->get<lv::Bool>()) {
            return eval(conseq, env);
        }
        else {
            return eval(alt, env);
        }
    }
    else {
        throw BadSpecialFormException("[BadSpecialFormException] `if` expression should be like (if pred conseq alt)");
    }
}

ValueP funcCond(ValueP pValue, EnvironmentP env) {
    auto list {pValue->get<lv::List>()};
    const auto size = list.size();
    for (size_t i {1}; i < size; ++i) {
        auto& elem = list[i];
        if (elem->get_type() != LispValType::List) {
            throw BadSpecialFormException(
                    "[BadSpecialFormException] A part of `cond` should be like (pred conseq), not", elem);
        }
        else if (elem->get<lv::List>().size() != 2) {
            throw BadSpecialFormException(
                    "[BadSpecialFormException] A part of `cond` should be like (pred conseq), not", elem);
        }

        auto pred {eval(elem->get<lv::List>()[0], env)};
        if (pred->get_type() == LispValType::Atom) {
            pred = make_shared_value(lv::Bool(true));
        }
        auto conseq {elem->get<lv::List>()[1]};
        CHECK_TYPE(pred, Bool);
        if (pred->get<lv::Bool>()) {
            return eval(conseq, env);
        }
    }
    throw BadSpecialFormException("[BadSpecialFormException] `cond` cannot finish in all exit");
}

ValueP funcDefine(ValueP pValue, EnvironmentP env) {
    auto list {pValue->get<lv::List>()};
    auto ident {list[1]};
    if (ident->get_type() == LispValType::Atom) {
        if (list.size() != 3) {
            throw BadSpecialFormException(
                    "[BadSpecialFormException] `define` expression to define a variable should be like (define ident value), but it is",
                    pValue);
        }

        if (env->contains(ident->get<lv::Atom>().str)) {
            throw IdentifierException("[IdentifierException] Duplicated identifier or keyword", ident);
        }

        auto value{eval(list[2], env)};
        env->put(ident->get<lv::Atom>().str, value);

        return value;
    }
    else if (ident->get_type() == LispValType::List) {
        auto name_param_list {ident->get<lv::List>()};

        auto func_name_vp {name_param_list[0]};
        if (func_name_vp->get_type() != LispValType::Atom) {
            throw BadSpecialFormException(
                    "[BadSpecialFormException] `define` expression to define a function should be like (define ([Atom]name params...) [body]), but it is",
                    ident);
        }
        auto func_name {func_name_vp->get<lv::Atom>().str};
        if (env->contains(func_name)) {
            throw IdentifierException("[IdentifierException] Duplicated identifier or keyword", func_name_vp);
        }

        ::std::deque<::std::string> params;
        ::std::unordered_set<::std::string> check_dup;
        for (size_t i {0}; i < name_param_list.size(); ++i) {
            auto& param_vp {name_param_list[i]};
            if (param_vp->get_type() != LispValType::Atom) {
                throw BadSpecialFormException(
                        "[BadSpecialFormException] Param list in function definition should be like (name [Atom]params...), but it is",
                        ident);
            }
            auto param_name {param_vp->get<lv::Atom>().str};
            if (!check_dup.contains(param_name)) {
                check_dup.insert(param_name);
            }
            else {
                throw IdentifierException("[IdentifierException] Identifiers cannot be duplicated in a closure, caused by", param_vp);
            }
            params.push_back(::std::move(param_name));
        }

        ::std::deque<ValueP> func_body;
        ::std::copy(list.cbegin() + 2, list.cend(), ::std::back_inserter(func_body));

        auto value {make_shared_value(lv::Function(params, func_body))};
        env->put(func_name, value);

        return value;
    }

    throw BadSpecialFormException("[BadSpecialFormException] `define` can only be used to define a variable or a function");
}

ValueP funcLet(ValueP pValue, EnvironmentP env) {
    auto list {pValue->get<lv::List>()};
    if (list[1]->get_type() != LispValType::List) {
        throw BadSpecialFormException("[BadSpecialFormException] `let` mapping list should be like ((var1 value1) (var2 value2) ...)), but it is", list[1]);
    }
    auto mapping_list {list[1]->get<lv::List>()};
    auto let_local_env = ::std::make_shared<Environment>(env);
    for (auto&& mapping : mapping_list) {
        if (mapping->get_type() != LispValType::List || mapping->get<lv::List>().size() != 2) {
            throw BadSpecialFormException("[BadSpecialFormException] `let` mapping item should be like (var value), but it is", mapping);
        }
        auto mapping_deter {mapping->get<lv::List>()};
        if (mapping_deter[0]->get_type() != LispValType::Atom) {
            throw BadSpecialFormException("[BadSpecialFormException] The first part of `let` mapping item should be Atom, but it is", mapping_deter[0]);
        }
        let_local_env->put(mapping_deter[0]->get<lv::Atom>().str, eval(mapping_deter[1], env));
    }

    for (size_t i {2}; i < list.size() - 1; ++i) {
        eval(list[i], let_local_env);
    }
    return eval(list.back(), let_local_env);
}

ValueP debugGlobalContext(EnvironmentP env) {
    if (global_context->env.empty()) {
        ::std::cout << "nil" << ::std::endl;
    }
    for (const auto& p : global_context->env) {
        ::std::cout << "(" << p.first << " . " << *p.second << ")" << ::std::endl;
    }
    return make_shared_value(lv::Bool(true));
}

ValueP debugEnvironment(EnvironmentP env) {
    for (const auto& p : env->env) {
        ::std::cout << "(" << p.first << " . " << *p.second << ")" << ::std::endl;
    }
    if (env->closure) debugEnvironment(env->closure);
    if (env->super_env) debugEnvironment(env->super_env);

    return make_shared_value(lv::Bool(true));
}

ValueP debugGetFunctionEnv(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, Function);

    if (a->get<lv::Function>().closure) {
        debugEnvironment(a->get<lv::Function>().closure);
    }

    return make_shared_value(lv::Bool(true));
}

ValueP numericUnopMinus(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    return make_shared_value(-a->get<lv::Number>());
}

ValueP boolBoolUnopNot(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, Bool);
    return make_shared_value(!a->get<lv::Bool>());
}

ValueP listCar(ValueP a, EnvironmentP env) {
    EVAL_A();
    switch (a->get_type()) {
        case LispValType::List:
            return a->get<lv::List>()[0];
        case LispValType::DottedList:
            return a->get<lv::DottedList>().first.front();
        default:
            throw TypeMismatchException("[TypeMismatchException] Excepted a `list` but received", a);
    }
}

ValueP listCdr(ValueP a, EnvironmentP env) {
    EVAL_A();
    switch (a->get_type()) {
        case LispValType::List: {
            lv::List cdr {a->get<lv::List>()};
            cdr.pop_front();
            return make_shared_value(cdr);
        }
        case LispValType::DottedList: {
            if (a->get<lv::DottedList>().first.size() == 1) {
                return a->get<lv::DottedList>().second;
            }
            lv::List first_cdr {a->get<lv::DottedList>().first};
            first_cdr.pop_front();
            return make_shared_value(lv::DottedList(first_cdr, a->get<lv::DottedList>().second));
        }
        default:
            throw TypeMismatchException("[TypeMismatchException] Excepted a `list` but received", a);
    }
}

ValueP numericBinopPlus(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() + b->get<lv::Number>());
}

ValueP numericBinopMinus(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() - b->get<lv::Number>());
}

ValueP numericBinopTimes(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() * b->get<lv::Number>());
}

ValueP numericBinopDivide(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() / b->get<lv::Number>());
}

ValueP numericBinopMod(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_INTEGER(a);
    CHECK_INTEGER(b);
    lv::Number aa {a->get<lv::Number>()}, bb {b->get<lv::Number>()};
    auto ia {static_cast<::std::int64_t>(aa)}, ib {static_cast<::std::int64_t>(bb)};
    if (bb > 0) {
        return make_shared_value(static_cast<lv::Number>(ia % ib));
    }
    else {
        return make_shared_value(static_cast<lv::Number>(ia * ib < 0 ? -1 : 1) * (-ib - ::emehcs::abs(ia) % ::emehcs::abs(ib)));
    }
}

ValueP numericBinopQuot(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(static_cast<lv::Number>(static_cast<::std::int64_t>(a->get<lv::Number>()) / static_cast<::std::int64_t>(b->get<lv::Number>())));
}

ValueP numericBinopRem(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_INTEGER(a);
    CHECK_INTEGER(b);
    return make_shared_value(static_cast<lv::Number>(static_cast<::std::int64_t>(a->get<lv::Number>()) % static_cast<::std::int64_t>(b->get<lv::Number>())));
}

ValueP numBoolBinopEq(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() == b->get<lv::Number>());
}

ValueP numBoolBinopL(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() < b->get<lv::Number>());
}

ValueP numBoolBinopLe(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() <= b->get<lv::Number>());
}

ValueP numBoolBinopG(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() > b->get<lv::Number>());
}

ValueP numBoolBinopGe(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() >= b->get<lv::Number>());
}

ValueP numBoolBinopNeq(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Number);
    CHECK_TYPE(b, Number);
    return make_shared_value(a->get<lv::Number>() != b->get<lv::Number>());
}

ValueP boolBoolBinopAnd(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Bool);
    CHECK_TYPE(b, Bool);
    return make_shared_value(a->get<lv::Bool>() && b->get<lv::Bool>());
}

ValueP boolBoolBinopOr(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, Bool);
    CHECK_TYPE(b, Bool);
    return make_shared_value(a->get<lv::Bool>() || b->get<lv::Bool>());
}



ValueP listCons(ValueP a, ValueP b, EnvironmentP env) {
    EVAL_AB();
    switch (b->get_type()) {
        case LispValType::List:
            if (b->get<lv::List>().empty()) {
                return make_shared_value(lv::List(1, a));
            }
            else {
                lv::List xs;
                xs.push_back(a);
                for (auto&& x : b->get<lv::List>()) {
                    xs.push_back(x);
                }
                return make_shared_value(xs);
            }
        case LispValType::DottedList: {
            lv::List xs;
            xs.push_back(a);
            for (auto&& x : b->get<lv::DottedList>().first) {
                xs.push_back(x);
            }
            return make_shared_value(lv::DottedList(xs, b->get<lv::DottedList>().second));
        }
        default:
            return make_shared_value(lv::DottedList(lv::List(1, a), b));
    }
}

static ValueP eqv_aux(ValueP, ValueP, EnvironmentP);

static ValueP eqv_aux(const lv::List& a, const lv::List& b, EnvironmentP env) {
    bool ret = true;
    if (a.size() != b.size()) {
        ret = false;
    }
    else {
        const auto len = a.size();
        for (size_t i {0}; i < len; ++i) {
            if (!eqv_aux(a[i], b[i], env)->get<lv::Bool>()) {
                ret = false;
                break;
            }
        }
    }
    return make_shared_value(ret);
}

ValueP eqv_aux(ValueP a, ValueP b, EnvironmentP env) {
    bool ret = true;

    if (a->get_type() != b->get_type()) {
        throw TypeMismatchException("[TypeMismatchException] Can't compare with different types, the second one is");
    }
    switch (a->get_type()) {
        case LispValType::Bool:
            ret = (a->get<lv::Bool>() == b->get<lv::Bool>());
            break;
        case LispValType::Number:
            ret = (a->get<lv::Number>() == b->get<lv::Number>());
            break;
        case LispValType::Char:
            ret = (a->get<lv::Char>() == b->get<lv::Char>());
            break;
        case LispValType::String:
            ret = (a->get<lv::String>() == b->get<lv::String>());
            break;
        case LispValType::Atom:
            ret = (a->get<lv::Atom>().str == b->get<lv::Atom>().str);
            break;
        case LispValType::DottedList:
            ret = boolBoolBinopAnd(eqv_aux(a->get<lv::DottedList>().first, b->get<lv::DottedList>().first, env),
                                   eqv_aux(a->get<lv::DottedList>().second, b->get<lv::DottedList>().second, env), env)->get<lv::Bool>();
            break;
        case LispValType::List:
            ret = eqv_aux(a->get<lv::List>(), b->get<lv::List>(), env)->get<lv::Bool>();
            break;
        case LispValType::Function:
            // TODO
        default:
            ret = false;
            break;
    }

    return make_shared_value(ret);
}

ValueP eqv(ValueP a, ValueP b, EnvironmentP env) {
    EVAL_AB();
    return eqv_aux(a, b, env);
}

ValueP loadFromFile(ValueP a, EnvironmentP env) {
    return loadFromFileWithPrompt(a, env);
}

ValueP loadFromFileWithPrompt(ValueP a, EnvironmentP env, bool prompt) {
    CHECK_TYPE(a, String);

    bool flagTest = false;

    auto filename {a->get<lv::String>()};
    size_t line_number = 0;
    ::std::string line;
    EnvironmentP old_env {::std::make_shared<Environment>(*global_context)};  // copy the surface layer
    try {
        ::std::ifstream ifs(filename, ::std::ios::in);
        if (!ifs.is_open()) {
            throw OpenFileFailure();
        }
        ::std::string expr;
        ::std::stack<char> sign_stack;
        int quote_counter {0};
        while (::std::getline(ifs, line)) {
            ++line_number;
            if (line_number == 1) {
                if (!line.empty() && line.front() == '*') {
                    line.erase(line.begin());
                    while (!line.empty() && ::std::isspace(line.front())) {
                        line.erase(line.begin());
                    }
                    if (line == "test") {
                        flagTest = true;
                    }
                    continue;
                }
            }
            for (size_t i {}; i < line.length(); ++i) {
                auto& ch {line[i]};
                if (pairs_sign.find(ch) != pairs_sign.end()) {
                    sign_stack.push(ch);
                }
                else {
                    switch (ch) {
                        case ')': case ']': case '}':
                            if (!sign_stack.empty() && pairs_sign[sign_stack.top()] == ch) {
                                sign_stack.pop();
                            }
                            else {
                                throw ParserError("[ParserError] Error signs paired, at L" + ::std::to_string(line_number) + "C" + ::std::to_string(i + 1));
                            }
                            break;
                        case '"':
                            quote_counter = ~quote_counter;
                            break;
                        case ';':
                            if (quote_counter == 0) {
                                line.erase(line.begin() + i, line.end());
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            if (!expr.empty()) {
                expr += "\n\t";
            }
            expr += line;

            if (!sign_stack.empty() || quote_counter) {
                continue;
            }

            while (::std::isspace(expr.back())) {
                expr.pop_back();
            }
            if (expr.empty()) {
                continue;
            }

            size_t cursor{0u};
            if (prompt) {
                ::std::cout << ">>> " << expr << ::std::endl;
                auto result {eval(parseExpr(expr, cursor), global_context)};
                ::std::cout << "=> " << *result << ::std::endl;
            }
            else if (flagTest) {
                auto parsed {parseExpr(expr, cursor)};
                if (parsed->get_type() == LispValType::List) {
                    auto lst {parsed->get<lv::List>()};
                    using namespace ::std::literals;
                    if (!lst.empty() && lst.front()->get_type() == LispValType::Atom && lst.front()->get<lv::Atom>().str.starts_with("assert"sv)) {
                        ::std::cout << "Test " << expr << ::std::endl;
                    }
                }
                eval(parsed, global_context);
            }
            else {
                eval(parseExpr(expr, cursor), global_context);
            }

            expr.clear();
        }
    }
    catch (OpenFileFailure& e) {
        ::std::cout << "Can't open file `" << filename << "` correctly" << ::std::endl;
        return make_shared_value(lv::Bool(false));
    }
    catch (LispException& e) {
        ::std::cout << "Trap at Line " << line_number << " in source file " << filename << ", " << ::std::endl;
        ::std::cout << "which is `" << line << "`" << ::std::endl;
        ::std::cout << e.what() << '\n';
        *global_context = *old_env;  // recovery status
        return make_shared_value(lv::Bool(false));
    }

    if (flagTest) {
        ::std::cout << "All test cases were correct and finished." << ::std::endl;
    }

    return make_shared_value(lv::Bool(true));
}

ValueP EqAssert(ValueP a, ValueP b, EnvironmentP env) {
    auto ret = eqv(a, b, env);
    if (!ret->get<lv::Bool>()) {
        ::std::cerr << "Assertion failed! " << *a << " /= " << *b << ::std::endl;
        ::std::exit(-1);
    }
    return ret;
}

}
