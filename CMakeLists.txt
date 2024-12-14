cmake_minimum_required(VERSION 3.16)

project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable target
add_executable(main 
    src/lexical_analyser/lexical_analyser.cpp
    src/lexical_analyser/token.cpp
    src/lexical_analyser/token_property.cpp
    src/main.cpp
    src/semantic_analyser/scope.cpp
    src/log/log.cpp
    src/log/log_thread_safe.cpp
    src/syntactic_analyser/grammar.cpp
    src/syntactic_analyser/syntactic_analyser.cpp
)

install(TARGETS main
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


set(CMAKE_CXX_FLAGS_DEBUG "-g -glldb")

# Include directories for headers
target_include_directories(main PRIVATE src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -pedantic-errors -Wconversion -Wsign-conversion -fsized-deallocation")
