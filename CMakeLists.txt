cmake_minimum_required(VERSION 2.8.7)
PROJECT(expression_evaluator)
add_definitions("-Wall")
cmake_policy(SET CMP0002 OLD)
cmake_policy(SET CMP0000 OLD)

set(HEADERS
    lexer.hpp
    parser.hpp
    evaluator.hpp)

set(SOURCES 
    main.cpp
    parser.cpp
    evaluator.cpp
    lexer.cpp)

ADD_EXECUTABLE(expression_evaluator ${HEADERS} ${SOURCES})
