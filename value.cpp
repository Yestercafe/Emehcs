#include <value.hpp>
#include <exception>
#include <eval.hpp>
#include <string>
#include <sstream>
#include <exception.hpp>

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

::std::string show(const lv::Function& func, bool should_prompt_type) {
    ::std::stringstream ss;
    ss << "(function (";
    bool firstIn = true;
    for (const auto& p : func.params) {
        if (firstIn) {
            firstIn = false;
        }
        else {
            ss << ' ';
        }
        ss << p;
    }
    ss << ") ...)";

    return ss.str();
}

::std::string show(const Value& value, bool should_prompt_type) {
    const auto type = value.get_type();
    ::std::stringstream ss;

    if (should_prompt_type) {
        ss << '[';
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
                ss << "Null";
                break;
        }
        ss << "]";
    }

    switch (type) {
        case LispValType::Atom:
            ss << value.get<lv::Atom>().str;
            break;
        case LispValType::List:
            ss << show(value.get<lv::List>(), should_prompt_type);
            break;
        case LispValType::DottedList:
            ss << show(value.get<lv::DottedList>(), should_prompt_type);
            break;
        case LispValType::Number:
            ss << ::std::to_string(value.get<lv::Number>());
            break;
        case LispValType::Char:
            ss << "'" << value.get<lv::Char>() << "'";
            break;
        case LispValType::String:
            ss << '"' << value.get<lv::String>() << '"';
            break;
        case LispValType::Bool:
            ss << (value.get<lv::Bool>() ? "#t" : "#f");
            break;
        case LispValType::Function:
            ss << show(value.get<lv::Function>(), should_prompt_type);
            break;
        default:
            ss << "<error>";
            break;
    }

    return ss.str();
}

void print_value(std::ostream& os, const Value& value, bool should_prompt_type) {
    os << show(value, false);
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
