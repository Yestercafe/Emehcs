#include <preload.hpp>
#include <tests.hpp>
#include <ui.hpp>
#include <environment.hpp>
#include <exception.hpp>

int main(int argc, char* argv[])
{
    using namespace ::emehcs;
    ::emehcs::initGlobalContext();

    if (argc == 1) {
        return ::ui::repl();
    }
    else {
        for (size_t i {1}; i < argc; ++i) {
            try {
                ::ui::readFromFile(argv[i]);
            } catch (::emehcs::LispException e) {
                ::std::cerr << "placeholder" << ::std::endl;
                ::std::exit(-1);
            }
        }
    }
}