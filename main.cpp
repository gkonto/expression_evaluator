#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "lexer.hpp"
#include "parse.hpp"
#include "evaluate.hpp"

/*----------------------------------------------*/
static int evaluate(const std::string &expr)
{
	//lexer
	Lexer lex;
	lex.process(expr);
	lex.display();
	std::vector<Token> token_list = lex.GetTokens();
	
	//parse
	Parser parse(token_list);
	parse.shunting_yard();
	std::list<Token> postfix = parse.GetPostfix();
	
	//evaluator
	Evaluator eval(postfix);
	std::vector<Token> stack_ = eval.evaluate();
	double value = atof(stack_[0].value.c_str());

	std::cout << "Calculated Number is: " << value << std::endl;
	std::cout << "----------------------" << std::endl;
	return value;
} /* evaluate */

/*----------------------------------------------*/
static void a_assert(const std::string &expr, double value)
{
	double eval = evaluate(expr);

	if (eval != value) {
		std::cout << "Test Failed in expression: " << expr << std::endl;
		assert(eval == value);
	}
} /* a_assert */

/*----------------------------------------------*/
static int eval_expression(const std::string &expr, double value)
{
	a_assert(expr, value);
	return 0;
} /* eval_expression */

/*----------------------------------------------*/
int main(void)
{
	eval_expression("14", 14);
	eval_expression("-14", -14);
	eval_expression("+14", 14);
	eval_expression("7+7", 14);
	eval_expression("-1+2", 1);
	std::cout << "ALL TESTS PASSED" << std::endl;
	return 0;

} /* main */

/*----------------------------------------------*/
