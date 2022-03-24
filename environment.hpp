#ifndef EMEHCS_ENVIRONMENT_HPP
#define EMEHCS_ENVIRONMENT_HPP

#include <preload.hpp>
#include <unordered_map>
#include <string>
#include <iterator>
#include <unordered_set>

namespace emehcs {

class Environment;

extern ::std::shared_ptr<Environment> global_context;
extern ::std::unordered_set<::std::string> Keywords;

class Environment {
  public:
    Environment() {
        super_env = nullptr;
    }
    // ::std::unordered_map default behavior is deep-copying
    Environment(EnvironmentP rhs) {
        super_env = rhs;
    }

    bool contains(::std::string ident) {
        if (Keywords.contains(ident)) {
            return true;
        }

        bool ret {false};
        auto i {this};

        while (i) {
            ret |= i->env.contains(ident);
            if (ret) {
                break;
            }
            i = i->super_env.get();
        }

        return ret;
    }

    bool put(::std::string ident, ValueP pValue) {
        if (Keywords.contains(ident)) {
            return false;
        }
        if (!env.contains(ident)) {
            env.emplace(ident, pValue);
            return true;
        }
        return false;
    }

    bool update(::std::string ident, ValueP pValue) {
        bool ret {false};
        auto i {this};

        while (i) {
            ret |= i->contains(ident);
            if (ret) {
                i->env[ident] = pValue;
                break;
            }
            i = i->super_env.get();
        }

        return ret;
    }

    ValueP get(::std::string ident) {
        auto i {this};

        while (i) {
            if (i->env.contains(ident)) {
                return i->env[ident];
            }
            i = i->super_env.get();
        }

        return nullptr;
    }

  private:
  public:
    ::std::unordered_map<::std::string, ValueP> env;
    EnvironmentP super_env;
};

void initGlobalContext();

}

#endif //EMEHCS_ENVIRONMENT_HPP
