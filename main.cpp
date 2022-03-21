#include <preload.hpp>
#include <value.hpp>
#include <parser.hpp>
#include <eval.hpp>
#include <string>
#include <exception.hpp>

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
        try {
            size_t cursor{0u};
            auto ret{emehcs::parseExpr(line, cursor)};
            if (ret.succ) {
                auto result{::emehcs::eval(ret.value_ptr)};
                if (result) {
                    ::std::cout << "eval[" << cnt << "]: " << *result << ::std::endl;
                    ++cnt;
                } else {
                    ::std::cout << "eval failed" << ::std::endl;
                }
            } else {
                ::std::cout << "parse failed" << ::std::endl;
            }
        }
        catch (emehcs::LispException& e) {
            ::std::cout << e.what() << '\n';
        }
        ::std::cout << ">>> ";
    }
    return 0;
}

int main()
{
//    ::test1();

    return repl();
}