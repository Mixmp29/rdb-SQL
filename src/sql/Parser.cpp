#include <sql/Parser.hpp>

#include <charconv>
#include <sstream>

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

}