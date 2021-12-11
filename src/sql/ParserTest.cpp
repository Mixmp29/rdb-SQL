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
    for (const auto& error : result.script_.statements_) {
        os << *error << "\n";
    }
    return os.str();
}

TEST(ParserSuite, ParserTest)
{
    EXPECT_EQ(40 + 2, 42);
}

TEST(ParserSuite, CreateTableTest)
{
    rdb::sql::Lexer lexer("CREATE TABLE Table;");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements = "";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, SelectStatemetTest)
{
    rdb::sql::Lexer lexer("SELECT user, name FROM Table;");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements = "";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, InsertStatementTest)
{
    rdb::sql::Lexer lexer("INSERT INTO Table (Colname) VALUES (12);");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements = "";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, DeleteStatementTest)
{
    rdb::sql::Lexer lexer("DELETE FROM Table;");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements = "";
    EXPECT_EQ(expected_statements, statements);
}

TEST(ParserSuite, DropTableTest)
{
    rdb::sql::Lexer lexer("DROP TABLE Table name");
    rdb::sql::Parser parser(lexer);
    const std::string statements = dump_statements(parser);
    const std::string expected_statements = "";
    EXPECT_EQ(expected_statements, statements);
}