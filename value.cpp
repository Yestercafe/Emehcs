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
                std::terminate();
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
            ss << "<function>";
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

ValueSharedPtr unpackNum(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::Number:
            return value_ptr;
        case LispValType::String:
            return make_shared_value(::std::stoll(value_ptr->get<lv::String>()));
        case LispValType::List:
            return eval(value_ptr);
        case LispValType::Atom: {
            auto var {eval(value_ptr)};
            if (var && var->get_type() == LispValType::Atom) {
                throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Number`", value_ptr);
            }
            return var;
        }
        default:
            throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Number`", value_ptr);
    }
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
        case LispValType::Atom: {
            auto var {eval(value_ptr)};
            if (var && var->get_type() == LispValType::Atom) {
                throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `String`", value_ptr);
            }
            return var;
        }
        default:
            throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `String`", value_ptr);
    }
}

ValueSharedPtr unpackBool(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::Bool:
            return value_ptr;
        case LispValType::List:
            return eval(value_ptr);
        case LispValType::Atom: {
            auto var {eval(value_ptr)};
            if (var && var->get_type() == LispValType::Atom) {
                throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Bool`", value_ptr);
            }
            return var;
        }
        default:
            throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Bool`", value_ptr);
    }
}

ValueSharedPtr unpackChar(ValueSharedPtr value_ptr) {
    switch (value_ptr->get_type()) {
        case LispValType::Char:
            return value_ptr;
        case LispValType::String:
            if (value_ptr->get<lv::String>().length() != 1) {
                throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Char`", value_ptr);
            }
            return make_shared_value(value_ptr->get<lv::String>().front());
        case LispValType::Atom: {
            auto var {eval(value_ptr)};
            if (var && var->get_type() == LispValType::Atom) {
                throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Char`", value_ptr);
            }
            return var;
        }
        default:
            throw TypeMismatchException("[TypeMismatchException] Can't unpack as a `Char`", value_ptr);
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
