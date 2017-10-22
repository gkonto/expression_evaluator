cmake_minimum_required(VERSION 2.8.7)
PROJECT(expression_evaluator)
add_definitions("-Wall")
cmake_policy(SET CMP0002 OLD)
cmake_policy(SET CMP0000 OLD)


set(HEADERS
   tools.hpp
   token.hpp
   lexer.hpp
   parser.hpp)

set(SOURCES 
    main.cpp
    token.cpp
    parser.cpp
    lexer.cpp
    tools.cpp)

ADD_EXECUTABLE(expression_evaluator ${HEADERS} ${SOURCES})
