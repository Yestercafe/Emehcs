#include <environment.hpp>
#include <eval.hpp>
#include <memory>

namespace emehcs {

EnvironmentP global_context;
::std::unordered_set<::std::string> Keywords {
    "lambda",
};

void initGlobalContext() {
    global_context = ::std::make_shared<Environment>();

    for (auto& [ident, _] : BuiltInFunctor) {
        Keywords.insert(ident);
    }
    for (auto& [ident, _] : UnaryOps) {
        Keywords.insert(ident);
    }
    for (auto& [ident, _] : BinaryOps) {
        Keywords.insert(ident);
    }
    for (auto& [ident, _] : FoldOps) {
        Keywords.insert(ident);
    }
}

}
