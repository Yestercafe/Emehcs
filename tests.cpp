#include <preload.hpp>
#include <value.hpp>
#include <environment.hpp>

void test_nested_closure_env()
{
    ::emehcs::EnvironmentP global = ::std::make_shared<::emehcs::Environment>();
    ::emehcs::EnvironmentP closure = ::std::make_shared<::emehcs::Environment>(global);
    ::emehcs::EnvironmentP sub_env = ::std::make_shared<::emehcs::Environment>(global, closure);

    global->put("a", ::emehcs::make_shared_value(::emehcs::lv::Number(1)));
    closure->put("b", ::emehcs::make_shared_value(::emehcs::lv::Number(2)));
    sub_env->put("a", ::emehcs::make_shared_value(::emehcs::lv::Number(3)));

    ::std::cout << ::emehcs::show(*sub_env->get("a")) << " " << ::emehcs::show(*sub_env->get("b")) << ::std::endl;
}
