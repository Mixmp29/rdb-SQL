#pragma once

// TODO(1): Домоделировать предметную область.
// TODO(2): Вспомогательные функции.
// TODO(3): Идентификаторы + тесты.
// TODO(4): Предпросмотр следующего токена.
// TODO(5): Ключевые слова + тесты.
// TODO(6): Целые числа + тесты.

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

}
