#ifndef EMECHS_PARSER_HPP
#define EMECHS_PARSER_HPP

#include <preload.hpp>
#include <defs.hpp>
#include <value.hpp>
#include <string_view>

namespace emehcs {

struct [[deprecated]] ParserReturns {
    bool succ;
    ::std::shared_ptr<Value> value_ptr;
    LispValType type;

    ParserReturns()
        : succ(false), value_ptr{}, type{}
    { }
    ParserReturns(bool succ, ::std::shared_ptr<Value> value_ptr, LispValType type)
        : succ{succ}, value_ptr{value_ptr}, type{type}
    { }
    ParserReturns(const ParserReturns&) = default;
};

bool skipSpaces(const ::std::string_view& s, size_t& cursor);
ValueP parseExpr(const ::std::string_view& s, size_t& cursor);
ValueP parseAtom(const ::std::string_view& s, size_t& cursor);
ValueP parseChar(const ::std::string_view& s, size_t& cursor);
ValueP parseString(const ::std::string_view& s, size_t& cursor);
ValueP parseNumber(const ::std::string_view& s, size_t& cursor);
ValueP parseDec(const ::std::string_view& s, size_t& cursor);
ValueP parseOct(const ::std::string_view& s, size_t& cursor);
ValueP parseHex(const ::std::string_view& s, size_t& cursor);
ValueP parseAnyList(const ::std::string_view& s, size_t& cursor);
ValueP parseList(const ::std::string_view& s, size_t& cursor);
[[deprecated]] ValueP parseDottedList(const ::std::string_view& s, size_t& cursor);
ValueP parseQuoted(const ::std::string_view& s, size_t& cursor);

}

#endif //EMECHS_PARSER_HPP
