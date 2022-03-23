#include <parser.hpp>
#include <cctype>
#include <sstream>
#include <memory>
#include <exception.hpp>

namespace emehcs {

bool skipSpaces(const ::std::string_view& s, size_t& cursor) {
    const auto len {s.length()};
    const auto origin_cursor {cursor};

    while (cursor < len && ::std::isspace(s[cursor])) {
        ++cursor;
    }

    return origin_cursor != cursor;
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
ValueSharedPtr parseExpr(const ::std::string_view& s, size_t& cursor) {
    ValueSharedPtr expr;

    bool canParse {
            (expr = parseNumber(s, cursor))
         || (expr = parseChar(s, cursor))
         || (expr = parseString(s, cursor))
         || (expr = parseAtom(s, cursor))
         || (expr = parseQuoted(s, cursor))
         || (expr = parseAnyList(s, cursor))
    };

    if (!canParse) {
        throw ParserError("[ParseError] this one can't be parsed -> " + ::std::string(s));
    }

    return expr;
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
ValueSharedPtr parseAtom(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;
    using ::std::isalpha;
    using ::std::isdigit;

    const auto len {s.length()};

    // first <- letter <|> symbol
    if (cursor >= len) {
        return nullptr;
    }
    char first {s[cursor++]};
    if (!isalpha(first) && !isSymbol(first)) {
        --cursor;
        return nullptr;
    }

    // rest <- many (letter <|> digit <|> symbol)
    char next {};
    stringstream rest;
    while (cursor < len) {
        next = s[cursor++];
        if (!isalpha(next) && !isdigit(next) && !isSymbol(next)) {
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
        return make_shared_value(lv::Bool(true));
    }
    else if (atom == "#f") {   // Bool false
        return make_shared_value(lv::Bool(false));
    }
    else {                     // regular atom
        return make_shared_value(lv::Atom(atom));
    }
}

/**
 *
 * @param s
 * @param cursor
 * @return do char '\''
              c <- noneOf "'"
              char '\''
              return $ Char c   (deprecated)
           use "#\?" instead
 */
ValueSharedPtr parseChar(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;

    const auto len {s.length()};
    char ch {};

    // s[cursor:cursor+3] is a `Char` pattern
    if (cursor + 2 >= len) {
        return nullptr;
    }
    if (s[cursor] == '#' && s[cursor + 1] == '\\') {
        if (s[cursor + 2] != '\\') {     // not an escaped char
            ch = s[cursor + 2];
        }
        else {
            if (cursor + 3 >= len) {
                throw ParserError("[ParserError] Can't process escaped chars in `Char` correctly");
            }
            else {
                auto fnd = EscapedMap.find(s[cursor + 3]);
                if (fnd != EscapedMap.cend()) {
                    ch = fnd->second;
                    // an escaped char makes an extra step
                    ++cursor;
                }
                else {
                    throw ParserError("[ParserError] Can't process escaped chars in `Char` correctly");
                }
            }
        }
        // step in 3
        cursor += 3;
    }
    else {
        return nullptr;
    }

    return make_shared_value(ch);
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
ValueSharedPtr parseString(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;

    const auto len {s.length()};
    const auto origin_cursor = cursor;

    // char '"'
    char quote {};
    if (cursor >= len) {
        return nullptr;
    }
    quote = s[cursor++];
    if (quote != '"') {
        --cursor;
        return nullptr;
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
                cursor = origin_cursor;
                throw ParserError("[ParserError] Can't process escaped chars in `String` correctly");
            }
        }
        else {
            x << ch;
        }
    }

    // char <- '"'
    quote = s[cursor++];

    return make_shared_value(lv::String(x.str()));
}

/**
 *
 * @param s
 * @param cursor
 * @return parseDec
       <|> try parseOct
       <|> try parseHex
 */
ValueSharedPtr parseNumber(const ::std::string_view& s, size_t& cursor) {
    ValueSharedPtr expr;

    bool _ {
            (expr = parseDec(s, cursor))
         || (expr = parseOct(s, cursor))
         || (expr = parseHex(s, cursor))
    };

    return expr;
}

/**
 *
 * @param s
 * @param cursor
 * @return do n <- many1 digit
              return (Number (read n))
 */
ValueSharedPtr parseDec(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;
    using ::std::isdigit;

    const auto len {s.length()};

    // n <- many1 digit
    char c {};
    stringstream n;
    if (cursor >= len) {
       return nullptr;
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
        return nullptr;
    }

    lv::Number number_n {::std::stoll(n.str())};
    return make_shared_value(number_n);
}

template<typename Pred>
static ValueSharedPtr parseHexAux(const ::std::string_view& s, size_t& cursor, char flag, Pred& pred) {
    using ::std::stringstream;

    const auto len {s.length()};

    // char <- '#'
    if (cursor >= len) return nullptr;
    char sharp = s[cursor++];
    if (sharp != '#') {
        --cursor;
        return nullptr;
    }

    // char <- 'x'
    if (cursor >= len) return nullptr;
    char f = s[cursor++];
    if (f != flag) {
        cursor -= 2;
        return nullptr;
    }

    // n <- many1 hexDigit
    char c {};
    stringstream n;
    if (cursor >= len) {
        cursor -= 2;
        return nullptr;
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
        return nullptr;
    }

    lv::Number number_n {::std::stoll(n.str(), nullptr, (flag == 'x' ? 16 : 8))};
    return make_shared_value(number_n);
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
ValueSharedPtr parseOct(const ::std::string_view& s, size_t& cursor) {
    return parseHexAux(s, cursor, 'o', isODigit);
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
ValueSharedPtr parseHex(const ::std::string_view& s, size_t& cursor) {
    return parseHexAux(s, cursor, 'x', isXDigit);
}

/**
 *
 * @param s
 * @param cursor
 * @return
 */
ValueSharedPtr parseAnyList(const ::std::string_view& s, size_t& cursor) {
    ValueSharedPtr expr {nullptr};
    const auto origin_cursor = cursor;

    if (s[cursor] == '(') {
        ++cursor;
        expr = parseList(s, cursor);
        if (expr == nullptr) {
            cursor = origin_cursor;
            throw ParserError("[ParserError] Can't parse `List` correct");
        }
        else if (s[cursor] != ')') {
            cursor = origin_cursor;
            throw ParserError("[ParserError] Can't parse `List` correct");
        }
        else {
            ++cursor;
        }
    }

    return expr;
}

/**
 *
 * @param s
 * @param cursor
 * @return liftM List $ sepBy parseExpr spaces
 */
ValueSharedPtr parseList(const ::std::string_view& s, size_t& cursor) {
    const auto origin_cursor {cursor};
    lv::List list;
    ValueSharedPtr tail {nullptr};
    bool isDottedList {false};
    skipSpaces(s, cursor);
    while (true) {
        ValueSharedPtr token {nullptr};
        try {
            token = parseExpr(s, cursor);
        }
        catch (LispException& e) {}

        if (token == nullptr) {
            // there are (4) conditions when parseExpr failed:
            // 1. the next character is dot, and `isDottedList` has not been set
            if (s[cursor] == '.' && !isDottedList) {
                // dot means this is a DottedList
                isDottedList = true;
                ++cursor;
                // detect if there is any space next
                if (!skipSpaces(s, cursor)) {
                    // if there is no space, this is not an alone dot and is an Atom whose prefix is a dot
                    tail = nullptr;    // direct to failed returns
                }
                else {
                    // continue to parse the tail of DottedList
                    continue;
                }
            }
            // 2. the next character is dot, but `isDottedList` has been set
            else if (s[cursor] == '.' && isDottedList) {
                // this means there are duplicated dots, List and DottedList parse failed
                tail = nullptr;        // direct to failed returns
            }
            // 3. the next character is not dot, and `isDottedList` has not been set
            else if (s[cursor] != '.' && !isDottedList) {
                // this means a List will terminate
                break;
            }
            // 4. the next character is not dot, but `isDottedList` has been set
            // = else if (s[cursor] != '.' && isDottedList) {
            else {
                // this means a DottedList will terminate
                break;
            }
            break;
        }
        // = else if (token.succ)

        // then there are 3 conditions:
        // 1. this is still a List
        if (!isDottedList) {
            list.push_back(token);
        }
        // 2. it is a DottedList, and tail is empty
        else if (tail == nullptr) {
            tail = token;
        }
        // 3. it is a DottedList, and tail will be duplicated
        else {
            tail = nullptr;      // failed returns
            break;
        }

        // spaces
        if (!skipSpaces(s, cursor)) {   // if there is no space back, the list parser will must be terminated
            // `skipSpaces` has popped out the terminated character, here is no work for cursor
            break;
        }
    }

    // Everyone doesn't pay attention to `list`, so there are 4 conditions:
    if (!isDottedList) {           // two conditions, exactly in this branch tail is always `nullptr`
        return make_shared_value(list);
    }
    else if (isDottedList && tail != nullptr) {     // a normal DottedList
        return make_shared_value(lv::DottedList(list, tail));
    }
    // = else if (isDottedList && tail == nullptr) {
    else {                         // the last branch, the DottedList doesn't have tail, this must be false
        cursor = origin_cursor;
        return nullptr;
    }
}

/**
 *
 * @param s
 * @param cursor
 * @return do
    head <- endBy parseExpr spaces
    tail <- char '.' >> spaces >> parseExpr
    return $ DottedList head tail
 */
[[deprecated]] ValueSharedPtr parseDottedList(const ::std::string_view& s, size_t& cursor) {
    using ::std::pair;
    using ::std::shared_ptr;

    const auto origin_cursor {cursor};

    // head <- endBy parseExpr spaces
    auto head = parseExpr(s, cursor);
    if (head == nullptr) {
        cursor = origin_cursor;
        return nullptr;
    }
    if (!skipSpaces(s, cursor)) {
        cursor = origin_cursor;
        return nullptr;
    }

    // tail <- char '.' >> spaces >> parseExpr
    if (s[cursor++] != '.') {
        cursor = origin_cursor;
        return nullptr;
    }
    if (!skipSpaces(s, cursor)) {
        cursor = origin_cursor;
        return {};
    }
    auto tail = parseExpr(s, cursor);
    if (tail == nullptr) {
        cursor = origin_cursor;
        return nullptr;
    }

    return nullptr;
    // return {true, make_shared_value(lv::DottedList(head.value_ptr, tail.value_ptr)), LispValType::DottedList};
}

/**
 *
 * @param s
 * @param cursor
 * @return do
    char '\''
    x <- parseExpr
    return $ List [Atom "quote", x]
 */
ValueSharedPtr parseQuoted(const ::std::string_view& s, size_t& cursor) {
    using ::std::stringstream;

    const auto len {s.length()};

    // char '\''
    char quote {};
    if (cursor >= len) {
        return nullptr;
    }
    quote = s[cursor++];
    if (quote != '\'') {
        --cursor;
        return {};
    }

    // x <- parseExpr
    auto ret = parseExpr(s, cursor);
    if (ret == nullptr) {
        --cursor;   // for quote
        return nullptr;
    }
    lv::List list;
    list.push_back(make_shared_value(lv::Atom("quote")));
    list.push_back(ret);

    return make_shared_value(list);
}

}
