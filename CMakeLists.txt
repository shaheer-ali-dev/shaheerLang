cmake_minimum_required(VERSION 3.10)
project(shaheerLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core language implementation (lexer, parser, interpreter, runtime)
add_library(shaheer_core
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/interpreter/Interpreter.cpp
    src/runtime/Environment.cpp
    src/runtime/Value.cpp
)
target_include_directories(shaheer_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Command-line interpreter
add_executable(shaheerLang src/main.cpp)
target_link_libraries(shaheerLang PRIVATE shaheer_core)

# Tests (cmake -S . -B build && cmake --build build && ctest --test-dir build)
option(SHAHEER_BUILD_TESTS "Build the test executables" ON)
if(SHAHEER_BUILD_TESTS)
    enable_testing()
    foreach(t lexer_tests parser_tests interpreter_tests)
        add_executable(${t} tests/${t}.cpp)
        target_link_libraries(${t} PRIVATE shaheer_core)
        add_test(NAME ${t} COMMAND ${t})
    endforeach()
endif()
