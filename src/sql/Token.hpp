#pragma once

#include <sql/Location.hpp>

#include <ostream>
#include <string_view>
#include <utility>

#define KwMax 9

namespace rdb::sql {

class Token {
public:
    enum class Kind {
        KwCreate,
        KwInsert,
        KwTable,
        KwSelect,
        KwFrom,
        KwWhere,
        KwInto,
        KwValues,
        KwDelete,
        KwDrop,
        KwTEXT,
        KwINT,
        KwREAL,
        Id,
        Eof,
        Unknown,
        Int,
        LParen,
        RParen,
        Comma,
        Semi,
        Lt,
        Lte,
        Str,
        Real
    };

    Token(Kind kind, std::string_view text, Location location)
        : kind_(kind), text_(text), location_(location)
    {
    }

    Kind kind() const
    {
        return kind_;
    }
    std::string_view text() const
    {
        return text_;
    }
    Location location() const
    {
        return location_;
    }

private:
    Kind kind_;
    std::string_view text_;
    Location location_;
};

std::string_view token_kind_to_str(Token::Kind kind);

std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace rdb::sql