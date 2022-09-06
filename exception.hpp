#ifndef EMEHCS_EXCEPTIONS_HPP
#define EMEHCS_EXCEPTIONS_HPP

#include <string>
#include <string_view>
#include <preload.hpp>
#include <sstream>
#include <value.hpp>

namespace emehcs {

using ::std::string;
using ::std::string_view;

class LispException {
  public:
    LispException(string str = "[Exception] A so-called exception caused by", ValueP pValue = nullptr)
        : pValue{pValue}
    {
        message = str + ": " + (pValue ? show(*pValue, true) : "<null>");
    }
    virtual ~LispException() = default;
    virtual string_view what() const {
        return message;
    }

  protected:
    string message;
    ValueP pValue;
};

using namespace ::std::literals;
class NumArgsException : public LispException {
    static constexpr string_view text_prefix {"[NumArgsException] Excepted "sv};
    static constexpr string_view text_suffix {" arg(s)"sv};
    static constexpr string_view at_least {"at least "sv};
  public:
    NumArgsException(int nArgs, ValueP pValue = nullptr, bool showAtLeast = false)
        : nArgs{nArgs}
    {
        ::std::stringstream ss;
        ss << text_prefix << (showAtLeast ? at_least : "") << nArgs << text_suffix;
        if (pValue) {
            ss << ": found values";
            const auto len {pValue->get<lv::List>().size()};
            for (size_t i {1}; i < len; ++i) {
                ss << " " << *pValue->get<lv::List>()[i];
            }
        }
        else {
            ss << "<null>";
        }

        LispException::message = ss.str();
        LispException::pValue = pValue;
    }

  private:
    int nArgs;
};

class TypeMismatchException : public LispException {
  public:
    TypeMismatchException(string str = "[TypeMismatch] Caused by", ValueP pValue = nullptr)
        : LispException(str, pValue)
    { }
};

class ParserError : public LispException {
  public:
    ParserError(string str = "[ParserError] Unknown error")
    {
        message = str;
    }
};

class BadSpecialFormException : public LispException {
  public:
    BadSpecialFormException(string str = "[BadSpecialFormException] Caused by", ValueP pValue = nullptr)
        : LispException(str, pValue)
    {
        if (pValue == nullptr) {
            message = str;
        }
    }
};

class NotFunctionException : public LispException {
  public:
    NotFunctionException(string str = "[NotFunctionException] Caused by", ValueP pValue = nullptr)
        : LispException(str, pValue)
    { }
};

class UnboundVarException : public LispException {
  public:
    UnboundVarException(string str = "[UnboundVarException] Caused by", ValueP pValue = nullptr)
        : LispException(str, pValue)
    { }
};

class IdentifierException : public LispException {
  public:
    IdentifierException(string str = "[IdentifierException] Caused by", ValueP pValue = nullptr)
            : LispException(str, pValue)
    { }
};

class OpenFileFailure : public ::std::exception {};


class BadValueRangeException : public LispException {
  public:
    BadValueRangeException(string str = "[BadValueRangeException] Caused by", ValueP pValue = nullptr)
            : LispException(str, pValue)
    { }
};

}

#endif //EMEHCS_EXCEPTIONS_HPP
