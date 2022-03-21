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
    LispException(string str = "A so-called exception caused by", ValueSharedPtr vsp = nullptr)
        : vsp{vsp}
    {
        message = str + ": " + (vsp ? show(*vsp) : "<null>");
    }
    virtual ~LispException() = default;
    virtual string_view what() const {
        return message;
    }

  protected:
    string message;
    ValueSharedPtr vsp;
};

class NumArgsException : public LispException {
    static constexpr string_view text_prefix {"Excepted "};
    static constexpr string_view text_suffix {" args: found values "};
  public:
    NumArgsException(int nArgs, ValueSharedPtr vsp = nullptr)
        : nArgs{nArgs}
    {
        ::std::stringstream ss;
        ss << text_prefix << nArgs << text_suffix;
        if (vsp) {
            ss << show(*vsp);
        }
        else {
            ss << "<null>";
        }

        LispException::message = ss.str();
        LispException::vsp = vsp;
    }

  private:
    int nArgs;
};

class TypeMismatchException : public LispException {
  public:
    TypeMismatchException(string str = "TypeMismatch caused by", ValueSharedPtr vsp = nullptr)
        : LispException(str, vsp)
    { }
};

class ParserError : public LispException {
  public:
    ParserError(string str = "ParserError")
    {
        message = str;
    }
};

class BadSpecialFormException : public LispException {
  public:
    BadSpecialFormException(string str = "BadSpecialFormException caused by", ValueSharedPtr vsp = nullptr)
        : LispException(str, vsp)
    { }
};

class NotFunctionException : public LispException {
  public:
    NotFunctionException(string str = "NotFunctionException caused by", ValueSharedPtr vsp = nullptr)
        : LispException(str, vsp)
    { }
};

class UnboundVarException : public LispException {
  public:
    UnboundVarException (string str = "UnboundVarException caused by", ValueSharedPtr vsp = nullptr)
        : LispException(str, vsp)
    { }
};

}

#endif //EMEHCS_EXCEPTIONS_HPP
