#ifndef EMEHCS_SCM_STRING_IPP
#define EMEHCS_SCM_STRING_IPP

#include <preload.hpp>
#include <environment.hpp>

namespace emehcs {

// ref: https://groups.csail.mit.edu/mac/ftpdir/scheme-7.4/doc-html/scheme_7.html

// Construction of Strings
ValueP strMakeString(ValueP a, ValueP b, EnvironmentP env);
ValueP funcString(ValueP a, EnvironmentP env);
ValueP strList2String(ValueP a, EnvironmentP env);
ValueP strString2List(ValueP a, EnvironmentP env);
ValueP strStringCopy(ValueP a, EnvironmentP env);

// Selecting String Components
ValueP strIsString(ValueP a, EnvironmentP env);
ValueP strStringLength(ValueP a, EnvironmentP env);
ValueP strIsStringNull(ValueP a, EnvironmentP env);
ValueP strStringRef(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringSetBang(ValueP a, ValueP b, ValueP c, EnvironmentP env);

// Comparison of Strings
ValueP strBoolBinopEq(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopL(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopLe(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopG(ValueP a, ValueP b, EnvironmentP env);
ValueP strBoolBinopGe(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringHash(ValueP a, EnvironmentP env);

// Alphabetic Case in Strings
ValueP strIsStringCapitalized(ValueP a, EnvironmentP env);
ValueP strIsStringUpperCase(ValueP a, EnvironmentP env);
ValueP strIsStringLowerCase(ValueP a, EnvironmentP env);
ValueP strStringCapitalize(ValueP a, EnvironmentP env);
ValueP strStringCapitalizeBang(ValueP a, EnvironmentP env);
ValueP strStringDowncase(ValueP a, EnvironmentP env);
ValueP strStringDowncaseBang(ValueP a, EnvironmentP env);
ValueP strStringUpcase(ValueP a, EnvironmentP env);
ValueP strStringUpcaseBang(ValueP a, EnvironmentP env);

// Cutting and Pasting Strings
ValueP strStringAppend(ValueP a, ValueP b, EnvironmentP env);
ValueP strSubstring(ValueP a, ValueP b, ValueP c, EnvironmentP env);
ValueP strStringHead(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringTail(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringPadLeft(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringPadRight(ValueP a, ValueP b, EnvironmentP env);
ValueP strStringTrimLeft(ValueP a, EnvironmentP env);
ValueP strStringTrimRight(ValueP a, EnvironmentP env);
ValueP strStringTrim(ValueP a, EnvironmentP env);

// Searching Strings
ValueP strIsSubstring(ValueP a, ValueP b, EnvironmentP env);

// Matching Strings
ValueP strIsStringPrefix(ValueP a, ValueP b, EnvironmentP env);
ValueP strIsStringSuffix(ValueP a, ValueP b, EnvironmentP env);

// Modification of Strings
ValueP strStringReplace(ValueP a, ValueP b, ValueP c, EnvironmentP env);
ValueP strStringReplaceBang(ValueP a, ValueP b, ValueP c, EnvironmentP env);

// Byte Vectors
ValueP strVector8bRef(ValueP a, ValueP b, EnvironmentP env);

}

#endif //EMEHCS_SCM_STRING_IPP
