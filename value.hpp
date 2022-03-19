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
using DottedList   = ::std::pair<::std::shared_ptr<Value>, ::std::shared_ptr<Value>>;
using Number       = ::std::int64_t;
using Char         = char;
using String       = ::std::string;
using Bool         = bool;
using Function     = void*;         // TODO

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

    template<typename T>
    Value(T&& value)
        : var{::std::forward<T>(value)}
    {  }

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

void print_value(std::ostream& os, const emehcs::lv::List& list, bool should_prompt_type);
void print_value(std::ostream& os, const emehcs::lv::DottedList& dotted_list, bool should_prompt_type);
void print_value(std::ostream& os, const emehcs::Value& value, bool should_prompt_type);

ValueSharedPtr unpackNum(ValueSharedPtr value_ptr);

}

std::ostream& operator<<(std::ostream& os, const emehcs::lv::List& rhs);
std::ostream& operator<<(std::ostream& os, const emehcs::lv::DottedList& rhs);
std::ostream& operator<<(std::ostream& os, const emehcs::Value& rhs);

#endif //EMECHS_VARIABLE_HPP
