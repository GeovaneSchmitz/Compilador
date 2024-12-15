cmake_minimum_required(VERSION 3.16)

project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable target
add_executable(main 
    src/binary_tree/binary_tree.cpp
    src/compiler/compiler.cpp
    src/lexical_analyser/lexical_analyser.cpp
    src/lexical_analyser/token_property.cpp
    src/lexical_analyser/token_type.cpp
    src/lexical_analyser/token.cpp
    src/log/log_thread_safe.cpp
    src/log/log.cpp
    src/main.cpp
    src/semantic_analyser/scope.cpp
    src/semantic_analyser/semantic_analyser.cpp
    src/syntactic_analyser/ast_node.cpp
    src/syntactic_analyser/non_terminal.cpp
    src/syntactic_analyser/syntactic_analyser.cpp
    src/syntactic_analyser/term_type.cpp
)

install(TARGETS main
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(main PRIVATE src)


set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Include directories for headers
target_include_directories(main PRIVATE src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -pedantic-errors -Wconversion -Wsign-conversion -Wno-unused-parameter -fsized-deallocation")
