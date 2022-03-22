#include <environment.hpp>
#include <eval.hpp>

namespace emehcs {

Environment global_context;
::std::unordered_set<::std::string> Keywords;

void initGlobalContext() {
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
