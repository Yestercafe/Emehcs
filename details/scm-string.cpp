#include <details/scm-string.ipp>
#include <defs.hpp>
#include <eval.hpp>
#include <cctype>
#include <string>
#include <algorithm>
#include <iterator>

namespace emehcs {

// Construction of Strings
ValueP strMakeString(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_INTEGER(a);
    CHECK_TYPE(b, Char);

    auto a_num {static_cast<::std::int64_t>(a->get<lv::Number>())};
    lv::Char b_char {b->get<lv::Char>()};
    lv::String ret(a_num, b_char);

    return make_shared_value(ret);
}

ValueP funcString(ValueP a, EnvironmentP env) {
    auto list {a->get<lv::List>()};
    const auto I {list.size()};
    lv::String ret;
    for (size_t i {1}; i < I; ++i) {
        CHECK_TYPE(list[i], Char);
        ret.push_back(list[i]->get<lv::Char>());
    }
    return make_shared_value(ret);
}

ValueP strList2String(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, List);
    auto list {a->get<lv::List>()};
    lv::String ret;
    for (auto&& c : list) {
        CHECK_TYPE(c, Char);
        ret.push_back(c->get<lv::Char>());
    }
    return make_shared_value(ret);
}

ValueP strString2List(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    lv::List ret;
    for (auto&& c : str) {
        ret.push_back(make_shared_value(lv::Char(c)));
    }
    return make_shared_value(ret);
}

ValueP strStringCopy(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    return make_shared_value(a->get<lv::String>());
}


// Selecting String Components
ValueP strIsString(ValueP a, EnvironmentP env) {
    return make_shared_value(lv::Bool(a->get_type() == LispValType::String));
}

ValueP strStringLength(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    return make_shared_value(static_cast<lv::Number>(str.length()));
}

ValueP strIsStringNull(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    return make_shared_value(lv::Bool(str.empty()));
}

ValueP strStringRef(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);
    auto str {a->get<lv::String>()};
    const auto I {str.length()};
    auto idx {static_cast<::std::int64_t>(b->get<lv::Number>())};
    if (idx < 0 || idx >= I) {
        throw BadValueRangeException("[BadValueRangeException] index " + ::std::to_string(idx) + " out of bound with", a);
    }
    return make_shared_value(lv::Char(str[idx]));
}

ValueP strStringSetBang(ValueP a, ValueP b, ValueP c, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);
    CHECK_TYPE(c, Char);
    auto str {a->get<lv::String>()};
    const auto I {str.length()};
    auto idx {static_cast<::std::int64_t>(b->get<lv::Number>())};
    if (idx < 0 || idx >= I) {
        throw BadValueRangeException("[BadValueRangeException] index " + ::std::to_string(idx) + " out of bound with", a);
    }
    return a;
}


// Comparison of Strings
ValueP strBoolBinopEq(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);
    return make_shared_value(a->get<lv::String>() == b->get<lv::String>());
}

ValueP strBoolBinopL(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);
    return make_shared_value(a->get<lv::String>() < b->get<lv::String>());
}

ValueP strBoolBinopLe(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);
    return make_shared_value(a->get<lv::String>() <= b->get<lv::String>());
}

ValueP strBoolBinopG(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);
    return make_shared_value(a->get<lv::String>() > b->get<lv::String>());
}

ValueP strBoolBinopGe(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);
    return make_shared_value(a->get<lv::String>() >= b->get<lv::String>());
}

ValueP strStringHash(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto s {a->get<lv::String>()};
    const auto I {s.length()};
    const int p = 31, m = 1e9 + 7;
    ::std::int64_t hash = 0;
    long p_pow = 1;
    for (int i = 0; i < I; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return make_shared_value(static_cast<lv::Number>(hash));
}


// Alphabetic Case in Strings
ValueP strIsStringCapitalized(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto True {make_shared_value(lv::Bool(true))};
    auto False {make_shared_value(lv::Bool(false))};

    auto str {a->get<lv::String>()};
    if (str.empty()) {
        return ::std::move(False);
    }
    return ::std::isupper(str.front()) ? ::std::move(True) : ::std::move(False);
}

ValueP strIsStringUpperCase(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    for (auto&& c : str) {
        if (!::std::isupper(c)) {
            return make_shared_value(lv::Bool(false));
        }
    }

    return make_shared_value(lv::Bool(true));
}

ValueP strIsStringLowerCase(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    for (auto&& c : str) {
        if (!::std::islower(c)) {
            return make_shared_value(lv::Bool(false));
        }
    }

    return make_shared_value(lv::Bool(true));
}

ValueP strStringCapitalize(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    str.front() = ::std::toupper(str.front());
    return make_shared_value(str);
}

ValueP strStringCapitalizeBang(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto& str {a->get<lv::String>()};
    str.front() = ::std::toupper(str.front());
    return a;
}

ValueP strStringDowncase(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    for (auto& c : str) {
        c = ::std::tolower(c);
    }

    return make_shared_value(str);
}

ValueP strStringDowncaseBang(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto& str {a->get<lv::String>()};
    for (auto& c : str) {
        c = ::std::tolower(c);
    }

    return a;
}

ValueP strStringUpcase(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    for (auto& c : str) {
        c = ::std::toupper(c);
    }

    return make_shared_value(str);
}

ValueP strStringUpcaseBang(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto& str {a->get<lv::String>()};
    for (auto& c : str) {
        c = ::std::toupper(c);
    }

    return a;
}


// Cutting and Pasting Strings
ValueP strStringAppend(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);

    return make_shared_value(a->get<lv::String>() + b->get<lv::String>());
}

ValueP strSubstring(ValueP a, ValueP b, ValueP c, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);
    CHECK_INTEGER(c);

    return make_shared_value(a->get<lv::String>().substr(static_cast<::std::int64_t>(b->get<lv::Number>()), static_cast<::std::int64_t>(c->get<lv::Number>())));
}

ValueP strStringHead(ValueP a, ValueP b, EnvironmentP env) {
    return strSubstring(a, make_shared_value(lv::Number(0)), b, env);
}

ValueP strStringTail(ValueP a, ValueP b, EnvironmentP env) {
    return strSubstring(a, b, make_shared_value(lv::Number(-1)), env);
}

ValueP strStringPadLeft(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);

    auto str {a->get<lv::String>()};
    auto bb {static_cast<::std::int64_t>(b->get<lv::Number>())};
    if (bb < 0) {
        throw BadValueRangeException("[BadValueRangeException] should be a non-negative number, but gotten", b);
    }
    const auto Len {str.length()};

    if (Len < bb) {
        return make_shared_value(::std::string(bb - Len, ' ') + str);
    }
    else if (Len == bb) {
        return make_shared_value(str);
    }
    else {
        return make_shared_value(str.substr(Len - bb));
    }
}

ValueP strStringPadRight(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);

    auto str {a->get<lv::String>()};
    auto bb {static_cast<::std::int64_t>(b->get<lv::Number>())};
    if (bb < 0) {
        throw BadValueRangeException("[BadValueRangeException] should be a non-negative number, but gotten", b);
    }
    const auto Len {str.length()};

    if (Len < bb) {
        return make_shared_value(str + ::std::string(bb - Len, ' '));
    }
    else if (Len == bb) {
        return make_shared_value(str);
    }
    else {
        return make_shared_value(str.substr(0, bb));
    }
}

ValueP strStringTrimLeft(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    const auto Len {str.length()};
    ::std::stringstream ss;

    bool theMostLeft = true;
    for (size_t i {0}; i < Len; ++i) {
        if (!theMostLeft) {
            ss << str[i];
            continue;
        }
        if (!::std::isspace(str[i])) {
            theMostLeft = false;
            ss << str[i];
        }
    }

    return make_shared_value(ss.str());
}

ValueP strStringTrimRight(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    auto str {a->get<lv::String>()};
    const auto Len {str.length()};
    ::std::stringstream ss;

    bool theMostLeft = true;
    for (::std::int64_t i {static_cast<::std::int64_t>(Len - 1)}; i >= 0; --i) {
        if (!theMostLeft) {
            ss << str[i];
            continue;
        }
        if (!::std::isspace(str[i])) {
            theMostLeft = false;
            ss << str[i];
        }
    }

    ::std::string reversed {ss.str()};
    ::std::reverse(::std::begin(reversed), ::std::end(reversed));

    return make_shared_value(reversed);
}

ValueP strStringTrim(ValueP a, EnvironmentP env) {
    CHECK_TYPE(a, String);
    return strStringTrimRight(strStringTrimLeft(a, env), env);
}
// Searching Strings
ValueP strIsSubstring(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);

    auto substring {a->get<lv::String>()};
    auto str {b->get<lv::String>()};

    return make_shared_value(lv::Bool(str.find(substring) != ::std::string::npos));
}

// Matching Strings
ValueP strIsStringPrefix(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);

    auto prefix {a->get<lv::String>()};
    auto str {b->get<lv::String>()};

    return make_shared_value(lv::Bool(str.starts_with(::std::string_view(prefix))));
}

ValueP strIsStringSuffix(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, String);

    auto suffix {a->get<lv::String>()};
    auto str {b->get<lv::String>()};

    return make_shared_value(lv::Bool(str.ends_with(::std::string_view(suffix))));
}

// Modification of Strings
ValueP strStringReplace(ValueP a, ValueP b, ValueP c, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, Char);
    CHECK_TYPE(c, Char);

    auto str {a->get<lv::String>()};
    auto ch1 {b->get<lv::Char>()};
    auto ch2 {c->get<lv::Char>()};
    ::std::replace(::std::begin(str), ::std::end(str), ch1, ch2);

    return make_shared_value(str);
}

ValueP strStringReplaceBang(ValueP a, ValueP b, ValueP c, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_TYPE(b, Char);
    CHECK_TYPE(c, Char);

    auto& str {a->get<lv::String>()};
    auto ch1 {b->get<lv::Char>()};
    auto ch2 {c->get<lv::Char>()};
    ::std::replace(::std::begin(str), ::std::end(str), ch1, ch2);

    return a;
}

// Byte Vectors
ValueP strVector8bRef(ValueP a, ValueP b, EnvironmentP env) {
    CHECK_TYPE(a, String);
    CHECK_INTEGER(b);

    auto str {a->get<lv::String>()};
    auto idx {static_cast<::std::int64_t>(b->get<lv::Number>())};
    const auto Len {str.length()};
    if (idx < 0 || idx >= Len) {
        throw BadValueRangeException("[BadValueRangeException] index " + ::std::to_string(idx) + " out of bound with", a);
    }

    lv::Char c {str[idx]};

    return make_shared_value(static_cast<lv::Number>(c));
}

}
