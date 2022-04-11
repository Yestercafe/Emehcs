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

::std::unordered_map<char, char> pairs_sign {
    {'(', ')'}, {'[', ']'}, {'{', '}'}, {'\"', '\"'},
};

int readFromFile(char* filename) {
    using namespace ::emehcs;

    ::std::ifstream ifs(filename, ::std::ios::in);
    if (!ifs.is_open()) {
        ::std::cerr << "Can't open " << filename << ::std::endl;
        return -1;
    }
    ::std::string expr;
    ::std::string line;
    std::stack<char> sign_stack;
    while (::std::getline(ifs, line)) {
        for (auto&& ch : line) {
            if (pairs_sign.find(ch) != pairs_sign.end()) {
                sign_stack.push(ch);
            }
            else {
                switch (ch) {
                    case ')': case ']': case '}': case '"':
                        if (pairs_sign[sign_stack.top()] == ch) {
                            sign_stack.pop();
                        } else {
                            throw ParserError("[ParserError] Error signs paired");
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if (!expr.empty()) expr += "\n\t";
        expr += line;

        if (!sign_stack.empty()) {
            continue;
        }

        while (::std::isspace(expr.back())) {
            expr.pop_back();
        }
        if (expr.empty()) {
            continue;
        }

        try {
            size_t cursor {0u};
            ::std::cout << ">>> " << expr << ::std::endl;
            auto result {eval(parseExpr(expr, cursor), global_context)};
            ::std::cout << "=> " << *result << ::std::endl;
        }
        catch (::emehcs::LispException& e) {
            ::std::cout << e.what() << '\n';
        }

        expr.clear();
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
        for (size_t i {1}; i < argc; ++i) {
            readFromFile(argv[i]);
        }
    }
}