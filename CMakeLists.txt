cmake_minimum_required(VERSION 3.16)

project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Compiler main.cpp
    context.h context.cpp
    symbol.h symbol.cpp
    astnode.h astnode.cpp
    parser.h parser.cpp
    token.h token.cpp)

include(GNUInstallDirs)
install(TARGETS Compiler
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
