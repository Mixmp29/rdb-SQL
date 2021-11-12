#include <sql/Parser.hpp>

#include <charconv>
#include <sstream>

namespace rdb::sql {


Parser::Result Parser::parse_sql_script() {
  Parser::Result result;
  while(true){
    Token next_token = lexer_.peek();
    if (next_token.kind() == Token::Kind::Eof) {
      break;
    }
    try {
      result.script_.statements_.push_back(parse_sql_statement());
    } catch (const SyntaxError& e){
      result.errors_.emplace_back(e.what());
      panic();
    }
  }
  return result;
}

void Parser::panic(){
  while (true) {
    const Token previev = lexer_.get();
    if (previev.kind() == Token::Kind::Eof || previev.kind() == Token::Kind::Semi){
      break;
    }
  }
}

StatementPtr Parser::parse_sql_statement() {
  const Token token = lexer_.peek();
  if (token.kind() == Token::Kind::KwInsert) {
    return parse_inset_statement();
  }
  if (token.kind() == Token::Kind::KwDrop) {
    return parse_drop_table_statement();
  }
  throw SyntaxError("Expected Drop or Insert");
}

Token Parser::fetch_token(Token::Kind expected_kind) {
  const Token token = lexer_.peek();
    if (token.kind() != expected_kind){
    throw SyntaxError("Expected " + std::string(token_kind_to_str(expected_kind)) + ", got " + 
    std::string(token_kind_to_str(token.kind())));
  }
  return lexer_.get();
}

DropTableStatementPtr Parser::parse_drop_table_statement() {
  fetch_token(Token::Kind::KwDrop);
  fetch_token(Token::Kind::KwTable);
  const Token table_name = fetch_token(Token::Kind::Id);
  fetch_token(Token::Kind::Semi);
  return std::make_unique<const DropTableStatement>(table_name.text());
}


}