#ifndef EMECHS_VARIABLE_HPP
#define EMECHS_VARIABLE_HPP

#include <string>
#include <deque>
#include <memory>
#include <cstdint>
#include <preload.hpp>
#include <variant>
#include <iostream>
#include <functional>
#include <environment.hpp>

namespace emehcs {

enum LispValType {
    Atom,
    List,
    DottedList,
    Number,
    Char,
    String,
    Bool,
    Function,
};

namespace lv {

struct Atom {
    ::std::string str;
    Atom() = default;
    Atom(const ::std::string& str)
        : str{str}
    { }
    Atom(const Atom&) = default;
    Atom(Atom&&) = default;
};
using List         = ::std::deque<::std::shared_ptr<Value>>;
using DottedList   = ::std::pair<::std::deque<::std::shared_ptr<Value>>, ::std::shared_ptr<Value>>;
using Number       = ::std::int64_t;
using Char         = char;
using String       = ::std::string;
using Bool         = bool;
struct Function {
    ::std::deque<::std::string> params;
    ::std::deque<ValueP> body;
    EnvironmentP closure;
    Function() = delete;
    Function(const Function&) = default;
    Function(::std::deque<::std::string>  params, const ::std::deque<ValueP>& body)
        : params{std::move(params)}, body{body}, closure{nullptr}
    { }
    Function(::std::deque<::std::string>  params, const ::std::deque<ValueP>& body, EnvironmentP env)
            : params{std::move(params)}, body{body}, closure{env}
    { }
};

}

class Value {
  private:
    using Varient =
            ::std::variant<lv::Atom
                         , lv::List
                         , lv::DottedList
                         , lv::Number
                         , lv::Char
                         , lv::String
                         , lv::Bool
                         , lv::Function
                         >;

    Varient var;

  public:
    Value()
        : var(nullptr)
    { }

    Value(const Value&) = default;
    Value(Value&&) = default;

    template<typename T>
    Value(T&& t)
        : var{::std::forward<T>(t)}
    { }

    LispValType get_type() const noexcept {
        return static_cast<LispValType>(var.index());
    }

    template<typename T>
    const T& get() const {
        return ::std::get<T>(var);
    }

    template<typename T>
    T& get() {
        return ::std::get<T>(var);
    }

    template<typename T>
    void set(T&& value) {
        var = std::forward<T>(value);
    }
};

::std::string show(const lv::List& list, bool = false);
::std::string show(const lv::DottedList& dotted_list, bool = false);
::std::string show(const lv::Function& func, bool = false);

void print_value(std::ostream& os, const emehcs::Value& value, bool = false);

}

std::ostream& operator<<(std::ostream& os, const emehcs::lv::List& rhs);
std::ostream& operator<<(std::ostream& os, const emehcs::lv::DottedList& rhs);
std::ostream& operator<<(std::ostream& os, const emehcs::Value& rhs);

#endif //EMECHS_VARIABLE_HPP
