#include <value.hpp>
#include <exception>

namespace emehcs {

void print_value(std::ostream& os, const lv::List& list) {
    os << '(';
    bool firstIn = true;
    for (const auto& p : list) {
        if (firstIn) firstIn = false;
        else os << ' ';
        print_value(os, *p);
    }
    os << ')';
}

void print_value(std::ostream& os, const lv::DottedList& dotted_list) {
    os << '(';
    print_value(os, *dotted_list.first);
    os << " . ";
    print_value(os, *dotted_list.second);
    os << ')';
}


void print_value(std::ostream& os, const Value& value) {
    const auto type = value.get_type();

    switch (type) {
        case LispValType::Atom:
            os << "Atom";
            break;
        case LispValType::List:
            os << "List";
            break;
        case LispValType::DottedList:
            os << "DottedList";
            break;
        case LispValType::Number:
            os << "Number";
            break;
        case LispValType::Char:
            os << "Char";
            break;
        case LispValType::String:
            os << "String";
            break;
        case LispValType::Bool:
            os << "Bool";
            break;
        case LispValType::Function:
            os << "Function";
            break;
        default:
            std::terminate();
    }
    os << ": ";

    switch (type) {
        case LispValType::Atom:
            os << value.get<lv::Atom>().name;
            break;
        case LispValType::List:
            print_value(os, value.get<lv::List>());
            break;
        case LispValType::DottedList:
            print_value(os, value.get<lv::DottedList>());
            break;
        case LispValType::Number:
            os << value.get<lv::Number>();
            break;
        case LispValType::Char:
            os << "'" << value.get<lv::Char>() << "'";
            break;
        case LispValType::String:
            os << '"' << value.get<lv::String>() << '"';
            break;
        case LispValType::Bool:
            os << (value.get<lv::Bool>() ? "#t" : "#f");
            break;
        case LispValType::Function:
            os << "<function>";
            break;
        default:
            std::terminate();
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