#include <preload.hpp>
#include <value.hpp>
#include <parser.hpp>
#include <eval.hpp>
#include <string>

void test1() {
    using namespace emehcs;

    lv::List list;
    list.emplace_back(::std::make_shared<Value>(lv::Number(1)));
    list.emplace_back(::std::make_shared<Value>(lv::String("hello")));
    list.emplace_back(::std::make_shared<Value>(lv::Bool(true)));

    lv::DottedList dotted_list;
    dotted_list.first = ::std::make_shared<Value>(lv::Number(2));
    dotted_list.second = ::std::make_shared<Value>(lv::Bool(false));
    list.emplace_back(::std::make_shared<Value>(::std::move(dotted_list)));

    ::std::cout << list << ::std::endl;
}

int main()
{
//    ::test1();

    ::std::string line;
    while (::std::getline(::std::cin, line)) {
        size_t cursor {0u};
        auto ret = emehcs::parseExpr(line, cursor);
        if (ret.succ) {
            auto result = ::emehcs::eval(ret.value_ptr);
            ::std::cout << *result << ::std::endl;
        }
        else {
            ::std::cout << "parse failed" << ::std::endl;
        }

    }

    return 0;
}