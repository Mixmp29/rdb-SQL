#include <sql/Parser.hpp>

#include <cassert>
#include <charconv>
#include <sstream>

namespace rdb::sql {

  class SyntaxError : public std::runtime_error {
    public:
    using std::runtime_error::runtime_error;
  };

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

InsertStatementPtr Parser::parse_inset_statement() {
  fetch_token(Token::Kind::KwInsert);
  fetch_token(Token::Kind::KwInto);
  const Token table_name = fetch_token(Token::Kind::Id);

  fetch_token(Token::Kind::LParen);

  std::vector<std::string_view> column_names;
  const Token first_column_name = fetch_token(Token::Kind::Id);
  column_names.push_back(first_column_name.text());

  while(lexer_.peek().kind() == Token::Kind::Comma){
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
  return std::make_unique<const  InsertStatement>(
         table_name.text(), column_names, values);
}

Value Parser::parse_value() {
  char* end = nullptr;
  Value result;
  const Token token = lexer_.peek();
    if (token.kind() == Token::Kind::Int){
      result = Value(std::strtol(token.text().data(), &end, 10));
      if (token.text().data() + token.text().size() != end){
        throw SyntaxError("Invalid symbols at\n" 
        + std::string(end - token.text().data(), ' ')
        + '^' + std::string(token.text().end() - end - 1, '~') + '\n');    
      }
    } else if (token.kind() == Token::Kind::Real){

      result = Value(std::strtof(token.text().data(), &end));
      if (token.text().data() + token.text().size() != end){
        throw SyntaxError("Invalid symbols at\n" 
        + std::string(end - token.text().data(), ' ')
        + '^' + std::string(token.text().end() - end - 1, '~') + '\n');    
      }

    } else {
      assert(token.kind() == Token::Kind::Str);
      return token.text(); 
    }
    throw SyntaxError("Expected Int or Real or String");
}

} // namespace rdb::sql