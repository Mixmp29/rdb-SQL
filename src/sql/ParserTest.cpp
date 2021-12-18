#include <sql/Lexer.hpp>
#include <sql/Parser.hpp>

#include <gtest/gtest.h>

#include <string>
#include <string_view>

std::string dump_statements(rdb::sql::Parser& parser)
{
    std::stringstream os;
    const rdb::sql::Parser::Result result = parser.parse_sql_script();
    for (const auto& statement : result.script_.statements_) {
        os << *statement << "\n";
    }
    for (const auto& error : result.errors_) {
        os << error << "\n";
    }
    return os.str();
}

TEST(ParserSuite, ParserTest)
{
    EXPECT_EQ(40 + 2, 42);
}

TEST(ParserSuite, CreateTableTest)
{
    rdb::sql::Lexer lexer(
            "CREATE TABLE Table (name1 TEXT, name2 REAL, name3 INT);"
            "CREATE TABLE Table (name1 REAL);"
            "CREATE TABLE Table (name1);"
            "CREATE TABLE Table (INT name1);");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements
            = "CreateTableStatement: TABLE - Table; ColumnDefs - name1 TEXT "
              "name2 REAL name3 INT\n"
              "CreateTableStatement: TABLE - Table; ColumnDefs - name1 REAL\n"
              "Expected KwINT, KwREAL or KwTEXT\n"
              "Expected Id, got KwINT\n";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, SelectStatemetTest)
{
    rdb::sql::Lexer lexer(
            "SELECT user1, user2, user3, name FROM Table WHERE 4 < 5;"
            "SELECT user, name FROM Table;"
            "SELECT user, name FROM;"
            "SELECT user, name FROM Table WHERE;");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements
            = "SelectStatement: TABLE - Table; ColumnList - user1 user2 user3 "
              "name; Expression - 4 Lt 5\n"
              "SelectStatement: TABLE - Table; ColumnList - user name\n"
              "Expected Id, got Semi\n"
              "Expected ColumnName or Value\n";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, InsertStatementTest)
{
    rdb::sql::Lexer lexer(
            "INSERT INTO Table (Colname) VALUES (6.13, 4, \"245\");"
            "INSERT INTO Table ColumnName1, ColumnName2 VALUES (3);"
            "INSERT INTO TABLE (ColumnName) VALUES(Value);");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements
            = "InsertStatement: TABLE - Table; Column Names - Colname; Values "
              "- 6.13 4 \"245\"\n"
              "Expected LParen, got Id\n"
              "Expected Id, got KwTable\n";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, DeleteStatementTest)
{
    rdb::sql::Lexer lexer(
            "DELETE FROM Table;"
            "DELETE FROM Table WHERE 4 < 5;"
            "DELETE FROM Table Expression;");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements
            = "DeleteStatement: TABLE - Table\n"
              "DeleteStatement: TABLE - Table; Expression - 4 Lt 5\n"
              "Expected Semi, got Id\n";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, DropTableTest)
{
    rdb::sql::Lexer lexer(
            "DROP TABLE Table;"
            "DROP trash Table;"
            "DROP DROP table;"
            "DROP TABLE TABLE");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements
            = "DropTableStatement: TABLE - Table\n"
              "Expected KwTable, got Id\n"
              "Expected KwTable, got KwDrop\n"
              "Expected Id, got KwTable\n";
    EXPECT_EQ(expected_statements, statements);
}