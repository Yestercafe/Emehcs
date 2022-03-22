#ifndef EMEHCS_ENVIRONMENT_HPP
#define EMEHCS_ENVIRONMENT_HPP

#include <preload.hpp>
#include <unordered_map>
#include <string>
#include <iterator>
#include <unordered_set>

namespace emehcs {

class Environment;

extern Environment global_context;
extern ::std::unordered_set<::std::string> Keywords;

class Environment {
  public:
    Environment() = default;
    // ::std::unordered_map default behavior is deep-copying
    Environment(const Environment& rhs) = default;

    bool contains(::std::string ident) {
        return env.contains(ident);
    }

    bool put(::std::string ident, ValueSharedPtr vsp) {
        if (Keywords.contains(ident)) {
            return false;
        }
        if (!contains(ident)) {
            env.emplace(ident, vsp);
            return true;
        }
        return false;
    }

    bool update(::std::string ident, ValueSharedPtr vsp) {
        if (contains(ident)) {
            env[ident] = vsp;
            return true;
        }
        return false;
    }

    ValueSharedPtr get(::std::string ident) {
        if (contains(ident)) {
            return env[ident];
        }
        return nullptr;
    }

  private:
    ::std::unordered_map<::std::string, ValueSharedPtr> env;
};

void initGlobalContext();


}

#endif //EMEHCS_ENVIRONMENT_HPP
