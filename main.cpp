#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "lexer.hpp"



static int evaluate(const std::string &expr)
{
	//lexer
	Lexer lex;
	lex.process(expr);
	lex.display();
	
	//parse
	
	//evaluator

	return atof(expr.c_str());
}

static int eval_expression(const std::string &expr, int value)
{

	assert(evaluate(expr) == value);

	return 0;
}

int main(void)
{
	eval_expression("14", 14);
	eval_expression("-14", -14);
	eval_expression("4+2", 6);
	std::cout << "ALL TESTS PASSED" << std::endl;
	return 0;

}
