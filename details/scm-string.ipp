#ifndef EMEHCS_SCM_STRING_IPP
#define EMEHCS_SCM_STRING_IPP

#include <preload.hpp>
#include <environment.hpp>

namespace emehcs {

ValueP strMakeString(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringAppend(ValueP a, ValueP b, EnvironmentP env);
ValueP strSubstring(ValueP a, ValueP b, ValueP c, EnvironmentP env);

}

#endif //EMEHCS_SCM_STRING_IPP
