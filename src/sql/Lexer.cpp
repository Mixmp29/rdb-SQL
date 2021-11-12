#include <sql/Lexer.hpp>

#include <cassert>
#include <cctype>
#include <unordered_map>

namespace rdb::sql {

Token Lexer::get() {
  if (next_token_.has_value()) {
    const Token token(next_token_.value());
    next_token_.reset();
    return token;
  }
	
  if (eof()){
    return Token(Token::Kind::Eof, "<EOF>", location_);
  }

  skip_spaces();
  if (isalpha(peek_char()) != 0){
    return get_id();
  }

  if(peek_char() == '('){
    Location begin(location_);
    get_char();
    return Token(Token::Kind::LParen, "(", begin);
  }

  if(peek_char() == ')'){
    Location begin(location_);
    get_char();
    return Token(Token::Kind::RParen, ")", begin);
  }

  if(peek_char() == ';'){
    Location begin(location_);
    get_char();
    return Token(Token::Kind::Semi, ";", begin);
  } 

    if(peek_char() == ','){
    Location begin(location_);
    get_char();
    return Token(Token::Kind::Comma, ",", begin);
  } 

  if ((isdigit(peek_char()) != 0) || (peek_char() == '-') || (peek_char() == '+')){
    return get_number();
  }

  if ((peek_char() == '<') || (peek_char() == '>') || (peek_char() == '!') || (peek_char() == '=')){
    return get_operator();
  }

  if (peek_char() == '\"'){
    return get_string();
  }
  
  Location begin(location_);
  get_char();
  return Token(Token::Kind::Unknown, input_.substr(begin.offset_, 1), begin);
}

bool Lexer::eof() const {
  return input_.size() == location_.offset_;
}

Token Lexer::peek() {
  if (next_token_.has_value()){
    return next_token_.value();
  }
  next_token_ = get();
  return next_token_.value();
}

char Lexer::peek_char() const {
  assert(!eof());
  return input_[location_.offset_];
}

char Lexer::get_char() {
  assert(!eof());
  if (peek_char() == '\n') {
    location_.col_ = 0;
    ++location_.row_;
  } else {
    ++location_.col_;
  }
  return input_[location_.offset_++];
}

void Lexer::skip_spaces() {
  while ((!eof()) && (isspace(peek_char()) != 0)){
    get_char();
  }
}


Token Lexer::get_id() {
  const Location begin(location_);
  while ((!eof()) && (isalnum(peek_char()) != 0)) {
    get_char();
  }

  const Location end(location_);
  const std::string_view text = input_.substr(begin.offset_, end.offset_ - begin.offset_);

  static const std::unordered_map<std::string_view, Token::Kind> text_to_kind = 
  {
    {"CREATE", Token::Kind::KwCreate},
    {"INSERT", Token::Kind::KwInsert},
    {"TABLE", Token::Kind::KwTable},
    {"SELECT", Token::Kind::KwSelect},
    {"FROM", Token::Kind::KwFrom},
    {"WHERE", Token::Kind::KwWhere},
    {"INTO", Token::Kind::KwInto},
    {"VALUES", Token::Kind::KwValues},
    {"DELETE", Token::Kind::KwDelete},
    {"DROP", Token::Kind::KwDrop}
  };

  const auto it = text_to_kind.find(text);
  if (it != text_to_kind.end()){
    return Token(it->second, text, begin);
  }

  return Token(Token::Kind::Id, text, begin);
}

Token Lexer::get_number() {
  const Location begin(location_);
  Token::Kind kind = Token::Kind::Int;
  if ((peek_char() == '+') || (peek_char() == '-')){
    get_char();
  }
  if (isdigit(peek_char()) != 0){
    if (peek_char() == '0'){
      get_char();
    } else {
      while ((!eof()) && (isdigit(peek_char()) != 0)){
        get_char();
      } 
    }
  } else { 
    kind = Token::Kind::Unknown;
  }
  if ((!eof()) && (peek_char() == '.')){
      get_char();
      if (isdigit(peek_char()) != 0){
        while ((!eof()) && (isdigit(peek_char()) != 0)){
          get_char();
        }
      kind = Token::Kind::Real;
    } else { 
      kind = Token::Kind::Unknown;
    }
  }
  const Location end(location_);
  const std::string_view text = input_.substr(begin.offset_, end.offset_ - begin.offset_);
  return Token(kind, text, begin);
}

Token Lexer::get_operator() {
  const Location begin(location_);
  Token::Kind kind;
  if (peek_char() == '<' || peek_char() == '>' || peek_char() == '='){
    get_char();
    if(peek_char() == '='){
      kind = Token::Kind::Lte;
      get_char();
    } else {
      kind = Token::Kind::Lt;
    }
  } else if (peek_char() == '!'){
    get_char();
    if(peek_char() == '='){
      kind = Token::Kind::Lte;
      get_char();
    } else {
      kind = Token::Kind::Unknown;
    }
  } else {
    kind = Token::Kind::Unknown;
  }
  const Location end(location_);
  const std::string_view text = input_.substr(begin.offset_, end.offset_ - begin.offset_);
  return Token(kind, text, begin);
}

Token Lexer::get_string(){
  const Location begin(location_);
  Token::Kind kind;
  get_char();
  while((!eof()) && (peek_char() != '"')){
    get_char();
    if(peek_char() == '\n') { break; }
  }
  if (peek_char() == '"'){
    get_char();
    kind = Token::Kind::Str;
  } else {
    kind = Token::Kind::Unknown;
  }

  const Location end(location_);
  const std::string_view text = input_.substr(begin.offset_, end.offset_ - begin.offset_);
  return Token(kind, text, begin);
}

}