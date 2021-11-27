#pragma once

#include <sql/Token.hpp>

#include <functional>
#include <optional>
#include <string_view>

namespace rdb::sql {

class Lexer {
 public:
  explicit Lexer(std::string_view input) : input_(input), location_(0, 0, 0) {}

  Token get();

  Token peek();

  std::string_view get_line(const Location& location) const;

 private:
  Token get_id();
  Token get_number();
  Token get_operator();
  Token get_string();
  
  bool eof() const;
  
  char peek_char() const;

  char get_char();

  void skip_spaces();

  std::string_view input_;
  Location location_;
  std::optional<Token> next_token_;
};

} // namespace rdb::sql