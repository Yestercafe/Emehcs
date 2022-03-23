#include <preload.hpp>
#include <value.hpp>
#include <parser.hpp>
#include <eval.hpp>
#include <string>
#include <exception.hpp>
#include <fstream>
#include <environment.hpp>

void test1() {
    using namespace emehcs;

    lv::List list;
    list.emplace_back(::std::make_shared<Value>(lv::Number(1)));
    list.emplace_back(::std::make_shared<Value>(lv::String("hello")));
    list.emplace_back(::std::make_shared<Value>(lv::Bool(true)));

    lv::DottedList dotted_list;
    dotted_list.first = lv::List(1, make_shared_value(lv::Number(2)));
    dotted_list.second = ::std::make_shared<Value>(lv::Bool(false));
    list.emplace_back(::std::make_shared<Value>(::std::move(dotted_list)));

    ::std::cout << list << ::std::endl;
}

int repl() {
    ::std::string line;
    ::std::cout << ">>> ";

    size_t cnt {0u};

    while (::std::getline(::std::cin, line)) {
        if (line == "(quit)" || line == "(exit)")
            break;
        try {
            size_t cursor {0u};
            auto expr {::emehcs::parseExpr(line, cursor)};
            auto result {::emehcs::eval(expr)};
            ::std::cout << "eval[" << cnt << "]: " << *result << ::std::endl;
            ++cnt;
        }
        catch (::emehcs::LispException& e) {
            ::std::cout << e.what() << '\n';
        }
        ::std::cout << ">>> ";
    }
    return 0;
}

int readFromFile(char* filename) {
    ::std::ifstream ifs(filename, ::std::ios::in);
    ::std::string line;
    while (::std::getline(ifs, line)) {
        try {
            size_t cursor {0u};
            ::std::cout << ">>> " << line << ::std::endl;
            auto result {::emehcs::eval(::emehcs::parseExpr(line, cursor))};
            ::std::cout << "=> " << *result << ::std::endl;
        }
        catch (::emehcs::LispException& e) {
            ::std::cout << e.what() << '\n';
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    using namespace ::emehcs;
    ::emehcs::initGlobalContext();


    if (argc == 1) {
        return repl();
    }
    else {
        for (size_t i = 1; i < argc; ++i) {
            readFromFile(argv[i]);
        }
    }
}