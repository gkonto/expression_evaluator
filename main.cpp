#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "lexer.hpp"
#include "parse.hpp"
#include "evaluate.hpp"

/*----------------------------------------------*/
static double evaluate(const std::string &expr)
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
	return value;
} /* evaluate */

/*----------------------------------------------*/
static void a_assert(const std::string &expr, double value)
{
	double eval = evaluate(expr);

	std::cout << "Comparing expr : " << eval << " with value : " << value << std::endl;

	if (eval != value) {
		std::cout << "Test Failed in expression: " << expr << std::endl;
		assert(eval == value);
	}
} /* a_assert */

/*----------------------------------------------*/
static int eval_expression(const std::string &expr, double value)
{
	a_assert(expr, value);
	std::cout << "------------------------------" << std::endl;
	return 0;
} /* eval_expression */

/*----------------------------------------------*/
int main(void)
{
	eval_expression("+14.0", 14);
	eval_expression("-14.0", -14);
	eval_expression("+14", 14);
	eval_expression("7+7", 14);
	eval_expression("-1+2", 1);
	eval_expression("(5+10*(3-9))", -55);
        eval_expression(".0 + .1 + .2 + .3 + .5 + .6", 1.7);
	eval_expression("1.0+3.0*2.0",1.0+3.0*2.0); 
	eval_expression("1.0-3.0*2.0",1.0-3.0*2.0); 
	eval_expression("1.0 *                           3.0+2.0", 5);
	eval_expression("-4.0-8.1", -4.0-8.1);
	eval_expression("1.0*3.0-2.0", 1.0*3.0-2.0);
	eval_expression("1.0+3.0/2.0", 1.0+3.0/2.0);
	eval_expression("1.0+(3.0*2.0-4.0)", 1.0+(3.0*2.0-4.0));
	eval_expression("1.0-(3.0*2.0+4.0)", 1.0-(3.0*2.0+4.0));
	eval_expression("2*3^2)", 18);

	std::cout << "ALL TESTS PASSED" << std::endl;
	return 0;

} /* main */

/*----------------------------------------------*/
