#include <preload.hpp>
#include <ui.hpp>
#include <value.hpp>
#include <parser.hpp>
#include <eval.hpp>
#include <string>
#include <exception.hpp>
#include <fstream>
#include <environment.hpp>
#include <stack>

namespace ui {

int repl() {
    using namespace ::emehcs;
    ::std::string line, expr;
    size_t counter {0u};
    size_t line_number {0u};
    ::std::stack<char> sign_stack;
    int quote_counter {0};

    ::std::cout << ">>> ";
    while (::std::getline(::std::cin, line)) {
        try {
            if (line == "(quit)" || line == "(exit)")
                break;
            for (size_t i {}; i < line.length(); ++i) {
                auto& ch {line[i]};
                if (pairs_sign.find(ch) != pairs_sign.end()) {
                    sign_stack.push(ch);
                }
                else {
                    switch (ch) {
                        case ')': case ']': case '}':
                            if (!sign_stack.empty() && pairs_sign[sign_stack.top()] == ch) {
                                sign_stack.pop();
                            }
                            else {
                                throw ParserError("[ParserError] Error signs paired, at L" + ::std::to_string(line_number) + "C" + ::std::to_string(i + 1));
                            }
                            break;
                        case '"':
                            quote_counter = ~quote_counter;
                        default:
                            break;
                    }
                }
            }

            if (!expr.empty()) {
                expr += "\n";
            }
            expr += line;
            ++line_number;

            if (!sign_stack.empty() || quote_counter) {
                ::std::cout << "#" << line_number << "> ";
                continue;
            }

            while (::std::isspace(expr.back())) {
                expr.pop_back();
            }
            if (expr.empty()) {
                ::std::cout << ">>> ";
                line_number = 0u;
                continue;
            }

            size_t cursor {0u};
            auto result {eval(parseExpr(expr, cursor), global_context)};
            ::std::cout << "eval[" << counter << "]: " << *result << ::std::endl;
            ++counter;

            expr.clear();
            line_number = 0u;
        }
        catch (::emehcs::LispException& e) {
            ::std::cout << e.what() << '\n';
            expr.clear();
            line_number = 0u;
        }
        ::std::cout << ">>> ";
    }
    return 0;
}

int readFromFile(char* filename) {
    auto ret = ::emehcs::loadFromFileWithPrompt(::emehcs::make_shared_value(::emehcs::lv::String(filename)), ::emehcs::global_context, true);

    return !ret->get<::emehcs::lv::Bool>();
}

}
