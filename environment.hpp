#ifndef EMEHCS_ENVIRONMENT_HPP
#define EMEHCS_ENVIRONMENT_HPP

#include <preload.hpp>
#include <unordered_map>
#include <string>
#include <iterator>
#include <unordered_set>
#include <value.hpp>
#include <iostream>

namespace emehcs {

class Environment;

extern ::std::shared_ptr<Environment> global_context;
extern ::std::unordered_set<::std::string> Keywords;

class Environment {
  public:
    Environment(EnvironmentP super_env = nullptr, EnvironmentP closure = nullptr)
        : super_env{super_env}, closure{closure}
    { }

    bool contains(::std::string ident) {
        if (Keywords.contains(ident)) {
            return true;
        }

        bool ret {false};
        auto i {this};
        while (i) {
            ret |= i->env.contains(ident);
            if (ret) {
                return ret;
            }

            if (i->closure) {
                ret |= i->closure->contains(ident);
                if (ret) {
                   return ret;
                }
            }

            i = i->super_env.get();
        }

        return ret;   // always false?
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
            ret |= env.contains(ident);
            if (ret) {
                env[ident] = pValue;
                return ret;
            }

            if (i->closure) {
                ret |= closure->update(ident, pValue);
                if (ret) {
                    return ret;
                }
            }

            i = i->super_env.get();
        }

        return ret;  // always false?
    }

    ValueP get(::std::string ident) {
        auto i {this};

        while (i) {
            if (i->env.contains(ident)) {
                return i->env[ident];
            }

            if (i->closure) {
                auto closure_ret {i->closure->get(ident)};
                if (closure_ret) {
                    return closure_ret;
                }
            }

            i = i->super_env.get();
        }

        return nullptr;
    }

    ::std::ostream& print(::std::ostream& os) {
        os << "{";
        for (auto&& [ident, value] : env) {
            os << ident << " => " << ::emehcs::show(*value) << ", ";
        }
        os << "}";
        return os;
    }

  private:
  public:
    ::std::unordered_map<::std::string, ValueP> env;
    EnvironmentP super_env;
    EnvironmentP closure;
};

void initGlobalContext();

}

#endif //EMEHCS_ENVIRONMENT_HPP
