#ifndef EMEHCS_SCM_IO_IPP
#define EMEHCS_SCM_IO_IPP

#include <preload.hpp>
#include <value.hpp>

namespace emehcs {

ValueP lispPrint(ValueP a, EnvironmentP env);
ValueP lispPrintLn(ValueP a, EnvironmentP env);
ValueP lispPrintFloating(ValueP a, ValueP b, EnvironmentP env);
ValueP lispPrintFloatingLn(ValueP a, ValueP b, EnvironmentP env);

}

#endif //EMEHCS_SCM_IO_IPP
