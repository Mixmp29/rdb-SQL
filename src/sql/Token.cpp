#include <sql/Token.hpp>

#include <ostream>

namespace rdb::sql {

std::string_view token_kind_to_str(Token::Kind token_kind)
{
    switch (token_kind) {
    case Token::Kind::KwCreate:
        return "KwCreate";
    case Token::Kind::KwInsert:
        return "KwInsert";
    case Token::Kind::KwTable:
        return "KwTable";
    case Token::Kind::KwSelect:
        return "KwSelect";
    case Token::Kind::KwFrom:
        return "KwFrom";
    case Token::Kind::KwWhere:
        return "KwWhere";
    case Token::Kind::KwInto:
        return "KwInto";
    case Token::Kind::KwValues:
        return "KwValues";
    case Token::Kind::KwDelete:
        return "KwDelete";
    case Token::Kind::KwDrop:
        return "KdDrop";
    case Token::Kind::Eof:
        return "Eof";
    case Token::Kind::Id:
        return "Id";
    case Token::Kind::Unknown:
        return "Unknown";
    case Token::Kind::Int:
        return "Int";
    case Token::Kind::LParen:
        return "LParen";
    case Token::Kind::RParen:
        return "RParen";
    case Token::Kind::Comma:
        return "Comma";
    case Token::Kind::Semi:
        return "Semi";
    case Token::Kind::Lt:
        return "Lt";
    case Token::Kind::Lte:
        return "Lte";
    case Token::Kind::Str:
        return "Str";
    case Token::Kind::Real:
        return "Real";
    }
    return "Unexpected";
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << token_kind_to_str(token.kind()) << " '" << token.text() << "' "
       << "Loc=" << token.location().row_ << ":" << token.location().col_;
    return os;
}

} // namespace rdb::sql