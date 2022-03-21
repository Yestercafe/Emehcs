#include <value.hpp>
#include <exception>
#include <eval.hpp>
#include <string>
#include <sstream>

namespace emehcs {

::std::string show(const lv::List& list, bool should_prompt_type) {
    ::std::stringstream ss;
    ss << '(';
    bool firstIn = true;
    for (const auto& p : list) {
        if (firstIn) firstIn = false;
        else ss << ' ';
        ss << show(*p, should_prompt_type);
    }
    ss << ')';
    return ss.str();
}

::std::string show(const lv::DottedList& dotted_list, bool should_prompt_type) {
    ::std::stringstream ss;
    ss << '(';
    bool firstIn = true;
    for (const auto& p : dotted_list.first) {
        if (firstIn) firstIn = false;
        else ss << ' ';
        ss << show(*p, should_prompt_type);
    }
    ss << " . " << show(*dotted_list.second, should_prompt_type) << ')';
    return ss.str();
}

::std::string show(const Value& value, bool should_prompt_type) {
    const auto type = value.get_type();
    ::std::stringstream ss;

    if (should_prompt_type) {
        switch (type) {
            case LispValType::Atom:
                ss << "Atom";
                break;
            case LispValType::List:
                ss << "List";
                break;
            case LispValType::DottedList:
                ss << "DottedList";
                break;
            case LispValType::Number:
                ss << "Number";
                break;
            case LispValType::Char:
                ss << "Char";
                break;
            case LispValType::String:
                ss << "String";
                break;
            case LispValType::Bool:
                ss << "Bool";
                break;
            case LispValType::Function:
                ss << "Function";
                break;
            default:
                std::terminate();
        }
        ss << ": ";
    }

    switch (type) {
        case LispValType::Atom:
            return value.get<lv::Atom>().str;
        case LispValType::List:
            return show(value.get<lv::List>(), should_prompt_type);
        case LispValType::DottedList:
            return show(value.get<lv::DottedList>(), should_prompt_type);
            break;
        case LispValType::Number:
            return ::std::to_string(value.get<lv::Number>());
        case LispValType::Char:
            ss << "'" << value.get<lv::Char>() << "'";
            return ss.str();
        case LispValType::String:
            ss << '"' << value.get<lv::String>() << '"';
            return ss.str();
        case LispValType::Bool:
            return (value.get<lv::Bool>() ? "#t" : "#f");
        case LispValType::Function:
            return "<function>";
        default:
            return "<error>";
    }
}

void print_value(std::ostream& os, const Value& value, bool should_prompt_type) {
    os << show(value, false);
}

ValueSharedPtr unpackNum(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::Number:
            return value_ptr;
        case LispValType::String:
            return make_shared_value(::std::stoll(value_ptr->get<lv::String>()));
        case LispValType::List:
            return eval(value_ptr);
        default:
            break;
    }
    return 0;
}

ValueSharedPtr unpackStr(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::String:
            return value_ptr;
        case LispValType::Number:
            return make_shared_value(show(*value_ptr));
        case LispValType::Bool:
            return make_shared_value(show(*value_ptr));
        case LispValType::List:
            return eval(value_ptr);
        default:
            return make_shared_value(lv::String("<notStringOrCantConvertToString>"));  // TODO exception
    }
}

ValueSharedPtr unpackBool(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::Bool:
            return value_ptr;
        case LispValType::List:
            return eval(value_ptr);
        default:
            return make_shared_value(false);      // TODO exception
    }
}

}

std::ostream& operator<<(std::ostream& os, const emehcs::lv::List& rhs) {
    emehcs::print_value(os, rhs);
    return os;
}

std::ostream& operator<<(std::ostream& os, const emehcs::lv::DottedList& rhs) {
    emehcs::print_value(os, rhs);
    return os;
}

std::ostream& operator<<(std::ostream& os, const emehcs::Value& rhs) {
    emehcs::print_value(os, rhs);
    return os;
}
