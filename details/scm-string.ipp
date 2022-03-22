#ifndef EMEHCS_SCM_STRING_IPP
#define EMEHCS_SCM_STRING_IPP

#include "preload.hpp"

namespace emehcs {

ValueSharedPtr strMakeString(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strStringAppend(ValueSharedPtr a, ValueSharedPtr b);
ValueSharedPtr strSubstring(ValueSharedPtr a, ValueSharedPtr b, ValueSharedPtr c);

}

#endif //EMEHCS_SCM_STRING_IPP
