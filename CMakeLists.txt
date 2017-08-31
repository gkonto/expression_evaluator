PROJECT(expression_evaluator)

set(HEADERS
    lexer.hpp
    parse.hpp
    evaluate.hpp)

set(SOURCES 
    main.cpp
    parse.cpp
    evaluate.cpp
    lexer.cpp)

ADD_EXECUTABLE(expression_evaluator ${HEADERS} ${SOURCES})

