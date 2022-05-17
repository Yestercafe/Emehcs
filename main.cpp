#include <preload.hpp>
#include <tests.hpp>
#include <ui.hpp>
#include <environment.hpp>

int main(int argc, char* argv[])
{
    using namespace ::emehcs;
    ::emehcs::initGlobalContext();

    if (argc == 1) {
        return ::ui::repl();
    }
    else {
        for (size_t i {1}; i < argc; ++i) {
            ::ui::readFromFile(argv[i]);
        }
    }
}