#include <sql/Parser.hpp>

#include <cassert>
#include <charconv>
#include <sstream>

namespace rdb::sql {

class SyntaxError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

Parser::Result Parser::parse_sql_script()
{
    Parser::Result result;
    while (true) {
        Token next_token = lexer_.peek();
        if (next_token.kind() == Token::Kind::Eof) {
            break;
        }
        try {
            result.script_.statements_.push_back(parse_sql_statement());
        } catch (const SyntaxError& e) {
            result.errors_.emplace_back(e.what());
            panic();
        }
    }
    return result;
}

void Parser::panic()
{
    while (true) {
        const Token previev = lexer_.get();
        if (previev.kind() == Token::Kind::Eof
            || previev.kind() == Token::Kind::Semi) {
            break;
        }
    }
}

void Parser::make_error_string(char* end, Token token)
{
    throw SyntaxError(
            "Invalid symbols at\n" + std::string(end - token.text().data(), ' ')
            + '^' + std::string(token.text().end() - end - 1, '~') + '\n');
}

StatementPtr Parser::parse_sql_statement()
{
    const Token token = lexer_.peek();
    if (token.kind() == Token::Kind::KwCreate) {
        return parse_create_table_statement();
    }
    if (token.kind() == Token::Kind::KwSelect) {
        return parse_select_statement();
    }
    if (token.kind() == Token::Kind::KwDelete) {
        return parse_delete_statement();
    }
    if (token.kind() == Token::Kind::KwDrop) {
        return parse_drop_table_statement();
    }
    if (token.kind() == Token::Kind::KwInsert) {
        return parse_insert_statement();
    }
    throw SyntaxError("Expected Drop or Insert");
}

Token Parser::fetch_token(Token::Kind expected_kind)
{
    const Token token = lexer_.peek();
    if (token.kind() != expected_kind) {
        throw SyntaxError(
                "Expected " + std::string(token_kind_to_str(expected_kind))
                + ", got " + std::string(token_kind_to_str(token.kind())));
    }
    return lexer_.get();
}

Token Parser::fetch_type_name()
{
    const Token token = lexer_.peek();
    if (token.kind() == Token::Kind::KwINT) {
        return fetch_token(Token::Kind::KwINT);
    }
    if (token.kind() == Token::Kind::KwREAL) {
        return fetch_token(Token::Kind::KwREAL);
    }
    if (token.kind() == Token::Kind::KwTEXT) {
        return fetch_token(Token::Kind::KwTEXT);
    }
    throw SyntaxError("Expected KwINT, KwREAL or KwTEXT");
}

std::string_view Parser::parse_operation()
{
    const Token token = lexer_.peek();
    std::string_view operation;
    if (token.kind() == Token::Kind::Lt) {
        fetch_token(Token::Kind::Lt);
        operation = "Lt";
    } else if (token.kind() == Token::Kind::Lte) {
        fetch_token(Token::Kind::Lte);
        operation = "Lte";
    } else {
        throw SyntaxError("Expected Lt or Lte");
    }

    return operation;
}

CreateTableStatementPtr Parser::parse_create_table_statement()
{
    fetch_token(Token::Kind::KwCreate);
    fetch_token(Token::Kind::KwTable);
    const Token table_name = fetch_token(Token::Kind::Id);

    fetch_token(Token::Kind::LParen);

    std::vector<std::string_view> column_defs;
    const Token first_column_name = fetch_token(Token::Kind::Id);
    const Token first_type_name = fetch_type_name();
    ;
    column_defs.push_back(first_column_name.text());
    column_defs.push_back(first_type_name.text());

    while (lexer_.peek().kind() == Token::Kind::Comma) {
        fetch_token(Token::Kind::Comma);
        const Token next_column_name = fetch_token(Token::Kind::Id);
        const Token next_type_name = fetch_type_name();
        column_defs.push_back(next_column_name.text());
        column_defs.push_back(next_type_name.text());
    }

    fetch_token(Token::Kind::RParen);

    fetch_token(Token::Kind::Semi);
    return std::make_unique<const CreateTableStatement>(
            table_name.text(), column_defs);
}

SelectStatementPtr Parser::parse_select_statement()
{
    fetch_token(Token::Kind::KwSelect);
    // SELECT user, name ...
    std::vector<std::string_view> column_list;
    const Token first_column_list = fetch_token(Token::Kind::Id);
    column_list.push_back(first_column_list.text());

    while (lexer_.peek().kind() == Token::Kind::Comma) {
        fetch_token(Token::Kind::Comma);
        const Token next_column_list = fetch_token(Token::Kind::Id);
        column_list.push_back(next_column_list.text());
    }

    fetch_token(Token::Kind::KwFrom);
    const Token table_name = fetch_token(Token::Kind::Id);

    if (lexer_.peek().kind() == Token::Kind::KwWhere) {
        fetch_token(Token::Kind::KwWhere);
        const auto expression = parse_expression();
        fetch_token(Token::Kind::Semi);

        return std::make_unique<const SelectStatement>(
                table_name.text(), column_list, expression);
    }
    fetch_token(Token::Kind::Semi);
    return std::make_unique<const SelectStatement>(
            table_name.text(), column_list, std::nullopt);
}

DeleteStatementPtr Parser::parse_delete_statement()
{
    fetch_token(Token::Kind::KwDelete);
    fetch_token(Token::Kind::KwFrom);
    const Token table_name = fetch_token(Token::Kind::Id);

    if (lexer_.peek().kind() == Token::Kind::KwWhere) {
        fetch_token(Token::Kind::KwWhere);
        const auto expression = parse_expression();
        fetch_token(Token::Kind::Semi);

        return std::make_unique<const DeleteStatement>(
                table_name.text(), expression);
    }
    fetch_token(Token::Kind::Semi);
    return std::make_unique<const DeleteStatement>(
            table_name.text(), std::nullopt);
}

DropTableStatementPtr Parser::parse_drop_table_statement()
{
    fetch_token(Token::Kind::KwDrop);
    fetch_token(Token::Kind::KwTable);
    const Token table_name = fetch_token(Token::Kind::Id);
    fetch_token(Token::Kind::Semi);
    return std::make_unique<const DropTableStatement>(table_name.text());
}

InsertStatementPtr Parser::parse_insert_statement()
{
    fetch_token(Token::Kind::KwInsert);
    fetch_token(Token::Kind::KwInto);
    const Token table_name = fetch_token(Token::Kind::Id);

    fetch_token(Token::Kind::LParen);

    std::vector<std::string_view> column_names;
    const Token first_column_name = fetch_token(Token::Kind::Id);
    column_names.push_back(first_column_name.text());

    while (lexer_.peek().kind() == Token::Kind::Comma) {
        fetch_token(Token::Kind::Comma);
        const Token next_column_name = fetch_token(Token::Kind::Id);
        column_names.push_back(next_column_name.text());
    }

    fetch_token(Token::Kind::RParen);
    fetch_token(Token::Kind::KwValues);
    fetch_token(Token::Kind::LParen);

    std::vector<Value> values;
    const Value first_value = parse_value();
    values.push_back(first_value);

    while (lexer_.peek().kind() == Token::Kind::Comma) {
        fetch_token(Token::Kind::Comma);
        const Value next_value = parse_value();
        values.push_back(next_value);
    }

    fetch_token(Token::Kind::RParen);
    fetch_token(Token::Kind::Semi);
    return std::make_unique<const InsertStatement>(
            table_name.text(), column_names, values);
}

Value Parser::parse_value()
{
    char* end = nullptr;
    const int base = 10;
    Value result;
    const Token token = lexer_.peek();
    if (token.kind() == Token::Kind::Int) {
        fetch_token(Token::Kind::Int);
        result = Value(std::strtol(token.text().data(), &end, base));
        if (token.text().data() + token.text().size() != end) {
            make_error_string(end, token);
        }
        return result;
    }
    if (token.kind() == Token::Kind::Real) {
        fetch_token(Token::Kind::Real);
        result = Value(std::strtof(token.text().data(), &end));
        if (token.text().data() + token.text().size() != end) {
            make_error_string(end, token);
        }
        return result;
    }
    if (token.kind() == Token::Kind::Str) {
        fetch_token(Token::Kind::Str);
        assert(token.kind() == Token::Kind::Str);
        return token.text();
    }
    throw SyntaxError("Expected Int or Real or String");
}

Operand Parser::parse_operand()
{
    const Token token = lexer_.peek();
    if (token.kind() == Token::Kind::Id) {
        std::string_view column_name = fetch_token(Token::Kind::Id).text();
        return column_name;
    }
    if (token.kind() == Token::Kind::Int || token.kind() == Token::Kind::Real
        || token.kind() == Token::Kind::Str) {
        Value value = parse_value();
        return value;
    }
    throw SyntaxError("Expected ColumnName or Value");
}

Expression Parser::parse_expression()
{
    Operand left = parse_operand();
    std::string_view operation = parse_operation();
    Operand right = parse_operand();

    return Expression(left, operation, right);
}

} // namespace rdb::sql