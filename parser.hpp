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
ValueSharedPtr parseExpr(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseAtom(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseChar(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseString(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseNumber(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseDec(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseOct(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseHex(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseAnyList(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseList(const ::std::string_view& s, size_t& cursor);
[[deprecated]] ValueSharedPtr parseDottedList(const ::std::string_view& s, size_t& cursor);
ValueSharedPtr parseQuoted(const ::std::string_view& s, size_t& cursor);

}

#endif //EMECHS_PARSER_HPP
