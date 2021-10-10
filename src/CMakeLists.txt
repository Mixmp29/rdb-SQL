set(main_name Sum)
set(test_name Test)
set(target_name LibSum)

add_executable(${main_name})
add_executable(${test_name})

add_library(${target_name} STATIC)

include(CompileOptions)

set_compile_options(${main_name})
set_compile_options(${test_name})
set_compile_options(${target_name})

target_sources(
  ${main_name}
  PRIVATE
    Sum/main.cpp
)

target_sources(
  ${test_name}
  PRIVATE
    LibSum/sum.test.cpp
)

target_sources(
  ${target_name}
  PRIVATE
    LibSum/sum.hpp
    LibSum/sum.cpp
)

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
  ${target_name}
  PRIVATE
    m
)

target_link_libraries(
  ${main_name}
  PRIVATE
    LibSum
    cxxopts
)

target_link_libraries(
  ${test_name}
  PRIVATE
    LibSum
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${test_name})