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

  explicit Parser(Lexer& lexer) : lexer_(lexer) {}

  Result parse_sql_script();

 private:
  void panic();
  Token fetch_token(Token::Kind expected_kind);
  StatementPtr parse_sql_statement();
  DropTableStatementPtr parse_drop_table_statement();
  InsertStatementPtr parse_inset_statement();
  Value parse_value();

  Lexer& lexer_;
};

} // namespace rdb::sql