set(test_name Test)
set(sql_name rdb)

add_executable(
    ${test_name}
    sql/LexerTest.cpp
)

add_library(
  ${sql_name} STATIC
  sql/Lexer.cpp
  sql/Lexer.hpp
  sql/Location.hpp
  sql/Token.cpp
  sql/Token.hpp
  sql/Parser.cpp
  sql/Parser.hpp
  sql/Script.hpp
  sql/Statements.cpp
  sql/Statements.hpp    
)

include(CompileOptions)

set_compile_options(${test_name})
set_compile_options(${sql_name})

target_include_directories(
  ${sql_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
  ${sql_name}
  PRIVATE
    m
)

target_link_libraries(
  ${test_name}
  PRIVATE
    rdb
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${test_name})
