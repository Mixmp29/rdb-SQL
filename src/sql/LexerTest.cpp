#include <sql/Lexer.hpp>

#include <gtest/gtest.h>

#include <string>
#include <string_view>

std::string dump_tokens(rdb::sql::Lexer& lexer)
{
    std::stringstream out;
    for (auto token = lexer.get();; token = lexer.get()) {
        out << token << "\n";
        if (token.kind() == rdb::sql::Token::Kind::Eof) {
            break;
        }
    }
    return out.str();
}

TEST(LexerSuite, LexerTest)
{
    EXPECT_EQ(40 + 2, 42);
}

TEST(LexerSuite, IdTest)
{
    std::string_view input = "a b1 aselect $";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "Id 'a' Loc=0:0\n"
              "Id 'b1' Loc=0:2\n"
              "Id 'aselect' Loc=0:5\n"
              "Unknown '$' Loc=0:13\n"
              "Eof '<EOF>' Loc=0:14\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, ParenTest)
{
    std::string_view input = "(a b1)";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "LParen '(' Loc=0:0\n"
              "Id 'a' Loc=0:1\n"
              "Id 'b1' Loc=0:3\n"
              "RParen ')' Loc=0:5\n"
              "Eof '<EOF>' Loc=0:6\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, SemiTest)
{
    std::string_view input = "(a b1);";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "LParen '(' Loc=0:0\n"
              "Id 'a' Loc=0:1\n"
              "Id 'b1' Loc=0:3\n"
              "RParen ')' Loc=0:5\n"
              "Semi ';' Loc=0:6\n"
              "Eof '<EOF>' Loc=0:7\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, LteTest)
{
    std::string_view input = "(a < b a <= t e != t q ! e);";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "LParen '(' Loc=0:0\n"
              "Id 'a' Loc=0:1\n"
              "Lt '<' Loc=0:3\n"
              "Id 'b' Loc=0:5\n"
              "Id 'a' Loc=0:7\n"
              "Lte '<=' Loc=0:9\n"
              "Id 't' Loc=0:12\n"
              "Id 'e' Loc=0:14\n"
              "Lte '!=' Loc=0:16\n"
              "Id 't' Loc=0:19\n"
              "Id 'q' Loc=0:21\n"
              "Unknown '!' Loc=0:23\n"
              "Id 'e' Loc=0:25\n"
              "RParen ')' Loc=0:26\n"
              "Semi ';' Loc=0:27\n"
              "Eof '<EOF>' Loc=0:28\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, StrTest)
{
    std::string_view input = "\"(a < b a <= t e != t q ! e);\"";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "Str '\"(a < b a <= t e != t q ! e);\"' Loc=0:0\n"
              "Eof '<EOF>' Loc=0:30\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, PeekTest)
{
    std::string_view input = "a b - c d";
    rdb::sql::Lexer lexer(input);
    std::stringstream str;
    str << lexer.peek() << "\n";
    str << lexer.peek() << "\n";
    str << lexer.peek() << "\n";
    std::string const expected_tokens
            = "Id 'a' Loc=0:0\n"
              "Id 'a' Loc=0:0\n"
              "Id 'a' Loc=0:0\n";
    EXPECT_EQ(expected_tokens, str.str());
}

TEST(LexerSuite, GetTest)
{
    std::string_view input = "a b + c d";
    rdb::sql::Lexer lexer(input);
    std::stringstream str;
    str << lexer.get() << "\n";
    str << lexer.get() << "\n";
    std::string const expected_tokens
            = "Id 'a' Loc=0:0\n"
              "Id 'b' Loc=0:2\n";
    EXPECT_EQ(expected_tokens, str.str());
}

TEST(LexerSuite, KeyWordTest)
{
    std::string_view input
            = "crEATE CREATE TABLE SELECT INSERT INTO WHERE VALUES DELETE FROM "
              "DROP";
    rdb::sql::Lexer lexer(input);
    std::string const tokens = dump_tokens(lexer);
    std::string const expected_tokens
            = "Id 'crEATE' Loc=0:0\n"
              "KwCreate 'CREATE' Loc=0:7\n"
              "KwTable 'TABLE' Loc=0:14\n"
              "KwSelect 'SELECT' Loc=0:20\n"
              "KwInsert 'INSERT' Loc=0:27\n"
              "KwInto 'INTO' Loc=0:34\n"
              "KwWhere 'WHERE' Loc=0:39\n"
              "KwValues 'VALUES' Loc=0:45\n"
              "KwDelete 'DELETE' Loc=0:52\n"
              "KwFrom 'FROM' Loc=0:59\n"
              "KdDrop 'DROP' Loc=0:64\n"
              "Eof '<EOF>' Loc=0:68\n";
    EXPECT_EQ(expected_tokens, tokens);
}

TEST(LexerSuite, NumberTest)
{
    rdb::sql::Lexer lexer(
            "+0 0.123456789 11 -11.121 +-. 0121 121. -plus 1.2.1");
    const std::string tokens = dump_tokens(lexer);
    const std::string expected_tokens
            = "Int '+0' Loc=0:0\n"
              "Real '0.123456789' Loc=0:3\n"
              "Int '11' Loc=0:15\n"
              "Real '-11.121' Loc=0:18\n"
              "Unknown '+' Loc=0:26\n"
              "Unknown '-.' Loc=0:27\n"
              "Int '0' Loc=0:30\n"
              "Int '121' Loc=0:31\n"
              "Unknown '121.' Loc=0:35\n"
              "Unknown '-' Loc=0:40\n"
              "Id 'plus' Loc=0:41\n"
              "Real '1.2' Loc=0:46\n"
              "Unknown '.' Loc=0:49\n"
              "Int '1' Loc=0:50\n"
              "Eof '<EOF>' Loc=0:51\n";
    EXPECT_EQ(expected_tokens, tokens);
}
