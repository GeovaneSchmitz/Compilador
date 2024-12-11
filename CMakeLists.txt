cmake_minimum_required(VERSION 3.16)

project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable target
add_executable(main 
    src/main.cpp
    src/semantic_analyser.hpp
    src/syntactic_analyser.hpp
    src/lexical_analyser.hpp
    src/log.hpp)

include(GNUInstallDirs)
install(TARGETS main
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
