cmake_minimum_required(VERSION 3.16)

project(
  Sum
  VERSION 1.0
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_program(CLANG_TIDY_EXE NAMES clang-tidy-12 clang-tidy-10 clang-tidy)
if (NOT CLANG_TIDY_EXE)
  message(WRITING "clang-tidy not found")
else()
  execute_process(
    COMMAND ${CLANG_TIDY_EXE} --version
    OUTPUT_VARIABLE CLANG_TIDY_VERSION)
  message("clang-tidy found:\n" ${CLANG_TIDY_VERSION})
endif()

add_subdirectory(src)
add_subdirectory(external)