#include <preload.hpp>
#include <value.hpp>
#include <parser.hpp>
#include <eval.hpp>
#include <string>
#include <exception.hpp>
#include <fstream>
#include <environment.hpp>
#include <tests.hpp>
#include <stack>

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
            auto result {::emehcs::eval(expr, ::emehcs::global_context)};
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
    auto ret = ::emehcs::loadFromFileWithPrompt(::emehcs::make_shared_value(::emehcs::lv::String(filename)), ::emehcs::global_context, true);

    return !ret->get<::emehcs::lv::Bool>();
}

int main(int argc, char* argv[])
{
    using namespace ::emehcs;
    ::emehcs::initGlobalContext();

    if (argc == 1) {
        return repl();
    }
    else {
        for (size_t i {1}; i < argc; ++i) {
            readFromFile(argv[i]);
        }
    }
}