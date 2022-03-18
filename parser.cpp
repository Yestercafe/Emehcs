#include <parser.hpp>
#include <cctype>
#include <sstream>
#include <debug.hpp>

namespace emehcs {

template<typename T>
auto make_shared_value(const T& t) {
    return ::std::make_shared<Value>(t);
}

void skipSpaces(const ::std::string_view& s, size_t& cursor) {
    const auto len {s.length()};

    while (cursor < len && ::std::isspace(s[cursor])) {
        ++cursor;
    }
}

/**
 *
 * @param s
 * @param cursor
 * @return parseNumber
       <|> parseChar
       <|> parseString
       <|> parseAtom
 */
ParserReturns parseExpr(const ::std::string_view& s, size_t& cursor) {
    skipSpaces(s, cursor);

    ParserReturns pr;

    bool _ {
          (pr = parseNumber(s, cursor)).succ
       || (pr = parseChar(s, cursor)).succ
       || (pr = parseString(s, cursor)).succ
       || (pr = parseAtom(s, cursor)).succ
    };

    return pr;
}

/**
 *
 * @param s
 * @param cursor
 * @return do first <- letter <|> symbol
              rest <- many (letter <|> digit <|> symbol)
              let atom = [first] ++ rest
              return $ case atom of
                         "#t" -> Bool True
                         "#f" -> Bool False
                         otherwise -> Atom atom
 */
ParserReturns parseAtom(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;
    using ::std::isalpha;
    using ::std::isdigit;

    const auto len {s.length()};

    // first <- letter <|> symbol
    if (cursor >= len) {
        return {};
    }
    char first {s[cursor++]};
    if (!isalpha(first) && !issymbol(first)) {
        --cursor;
        return {};
    }

    // rest <- many (letter <|> digit <|> symbol)
    char next {};
    stringstream rest;
    while (cursor < len) {
        next = s[cursor++];
        if (!isalpha(next) && !isdigit(next) && !issymbol(next)) {
            --cursor;
            break;
        }
        rest << next;
    }

    // let atom = [first] ++ rest
    stringstream atom_ss;
    atom_ss << first << rest.str();
    const auto& atom = atom_ss.str();

    // return
    if (atom == "#t") {        // Bool true
        return {true, make_shared_value(lv::Bool(true)), LispValType::Bool};
    }
    else if (atom == "#f") {   // Bool false
        return {true, make_shared_value(lv::Bool(false)), LispValType::Bool};
    }
    else {                     // regular atom
        return {true, make_shared_value(lv::Atom(atom)), LispValType::Atom};
    }

    return {};
}

/**
 *
 * @param s
 * @param cursor
 * @return do char '\''
              c <- noneOf "'"
              char '\''
              return $ Char c
 */
ParserReturns parseChar(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;

    const auto len {s.length()};

    // char '\''
    char quote {};
    if (cursor >= len) {
        return {};
    }
    quote = s[cursor++];
    if (quote != '\'') {
        --cursor;
        return {};
    }

    // c <- noneOf "'"
    if (cursor >= len) {
        --cursor;
        return {};
    }
    char ch {};
    ch = s[cursor++];
    if (ch == '\'') {
        cursor -= 2;
        return {};
    }
    else if (ch == '\\') {              // escaped char
        char escaped = s[cursor++];
        auto fnd = EscapedMap.cbegin();
        if ((fnd = EscapedMap.find(escaped)) != EscapedMap.cend()) {
            ch = fnd->second;
        }                 // not a correct escaped char
        else {
            // TODO throws exception
            ::std::cerr << "Escaped char error" << ::std::endl;
            cursor -= 3;
            LOG(::std::cerr, "cursor=", cursor);
            return {};
        }
    }

    if (cursor >= len) {
        cursor -= 2;
        return {};
    }
    quote = s[cursor++];
    if (quote != '\'') {
        cursor -= 3;
        return {};
    }

    return {true, make_shared_value(ch), LispValType::Char};
}

/**
 *
 * @param s
 * @param cursor
 * @return do char '"'
              x <- many (noneOf "\"")
              char '"'
              return $ String x
 */
ParserReturns parseString(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;

    const auto len {s.length()};

    // char '"'
    char quote {};
    if (cursor >= len) {
        return {};
    }
    quote = s[cursor++];
    if (quote != '"') {
        --cursor;
        return {};
    }

    // x <- many (noneOf "\"")
    char ch {};
    stringstream x;
    while (cursor < len) {
        ch = s[cursor++];
        if (ch == '"') {
            --cursor;
            break;
        }
        else if (ch == '\\') {              // escaped char
            char escaped = s[cursor++];
            auto fnd = EscapedMap.cbegin();
            if ((fnd = EscapedMap.find(escaped)) != EscapedMap.cend()) {
                x << fnd->second;
            }                 // not a correct escaped char
            else {
                x << '\\' << escaped;
            }
        }
        else {
            x << ch;
        }
    }

    // char <- '"'
    quote = s[cursor++];

    return {true, make_shared_value(lv::String(x.str())), LispValType::String};
}

/**
 *
 * @param s
 * @param cursor
 * @return parseDec
       <|> try parseOct
       <|> try parseHex
 */
ParserReturns parseNumber(const ::std::string_view& s, size_t& cursor) {
    ParserReturns pr;

    bool _ {
            (pr = parseDec(s, cursor)).succ
         || (pr = parseOct(s, cursor)).succ
         || (pr = parseHex(s, cursor)).succ
    };

    return pr;
}

/**
 *
 * @param s
 * @param cursor
 * @return do n <- many1 digit
              return (Number (read n))
 */
ParserReturns parseDec(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;
    using ::std::isdigit;

    const auto len {s.length()};

    // n <- many1 digit
    char c {};
    stringstream n;
    if (cursor >= len) {
       return {};
    }
    while (cursor < len) {
        c = s[cursor++];
        if (!isdigit(c)) {
            --cursor;
            break;
        }
        n << c;
    }

    if (n.str().length() == 0) {
        return {};
    }

    lv::Number number_n {::std::stoll(n.str())};
    return {true, make_shared_value(number_n), LispValType::Number};
}

template<typename Pred>
static ParserReturns parseHexAux(const ::std::string_view& s, size_t& cursor, char flag, Pred& pred) {
    using ::std::stringstream;

    const auto len {s.length()};

    // char <- '#'
    if (cursor >= len) return {};
    char sharp = s[cursor++];
    if (sharp != '#') {
        --cursor;
        return {};
    }

    // char <- 'x'
    if (cursor >= len) return {};
    char f = s[cursor++];
    if (f != flag) {
        cursor -= 2;
        return {};
    }

    // n <- many1 hexDigit
    char c {};
    stringstream n;
    if (cursor >= len) {
        cursor -= 2;
        return {};
    }
    while (cursor < len) {
        c = s[cursor++];
        if (!pred(c)) {
            --cursor;
            break;
        }
        n << c;
    }

    if (n.str().length() == 0) {
        cursor -= 2;
        return {};
    }

    lv::Number number_n {::std::stoll(n.str(), nullptr, (flag == 'x' ? 16 : 8))};
    return {true, make_shared_value(number_n), LispValType::Number};
}

/**
 *
 * @param s
 * @param cursor
 * @return do char '#'
              char 'o'
              rest <- many1 octDigit
              return (Number (fst (readOct rest !! 0)))
 */
ParserReturns parseOct(const ::std::string_view& s, size_t& cursor) {
    return parseHexAux(s, cursor, 'o', isodigit);
}

/**
 *
 * @param s
 * @param cursor
 * @return do char '#'
              char 'x'
              rest <- many1 hexDigit
              return (Number (fst (readHex rest !! 0)))
 */
ParserReturns parseHex(const ::std::string_view& s, size_t& cursor) {
    return parseHexAux(s, cursor, 'x', isxdigit);
}

}
