#ifndef EMECHS_PARSER_HPP
#define EMECHS_PARSER_HPP

#include <preload.hpp>
#include <defs.hpp>
#include <value.hpp>
#include <string_view>

namespace emehcs {

struct ParserReturns {
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

void skipSpaces(const ::std::string_view& s, size_t& cursor);
ParserReturns parseExpr(const ::std::string_view& s, size_t& cursor);
ParserReturns parseAtom(const ::std::string_view& s, size_t& cursor);
ParserReturns parseChar(const ::std::string_view& s, size_t& cursor);
ParserReturns parseString(const ::std::string_view& s, size_t& cursor);
ParserReturns parseNumber(const ::std::string_view& s, size_t& cursor);
ParserReturns parseDec(const ::std::string_view& s, size_t& cursor);
ParserReturns parseOct(const ::std::string_view& s, size_t& cursor);
ParserReturns parseHex(const ::std::string_view& s, size_t& cursor);

}

#endif //EMECHS_PARSER_HPP
