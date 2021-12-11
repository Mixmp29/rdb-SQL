#pragma once

#include <sql/Lexer.hpp>
#include <sql/Script.hpp>

#include <string>
#include <vector>

namespace rdb::sql {

class Parser {
public:
    struct Result {
        Script script_;
        std::vector<std::string> errors_;
    };

    explicit Parser(Lexer& lexer) : lexer_(lexer)
    {
    }

    Result parse_sql_script();

private:
    void panic();
    static void make_error_string(char* end, Token token);
    Token fetch_token(Token::Kind expected_kind);
    void fetch_type_name();
    std::string_view parse_operation();
    StatementPtr parse_sql_statement();
    CreateTableStatementPtr parse_create_table_statement();
    SelectStatementPtr parse_select_statement();
    DeleteStatementPtr parse_delete_statement();
    DropTableStatementPtr parse_drop_table_statement();
    InsertStatementPtr parse_inset_statement();
    Value parse_value();
    Operand parse_operand();
    Expression parse_expression();

    Lexer& lexer_;
};

} // namespace rdb::sql