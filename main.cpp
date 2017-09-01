#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
/*----------------------------------------------*/
static double evaluate(const std::string &expr)
{
	//lexer
	Lexer lex;
	lex.process(expr);
	lex.display();
	std::vector<Token> token_list = lex.getTokens();
	
	//parse
	Parser parse(token_list);
	parse.shuntingYard();
	std::list<Token> postfix = parse.getPostFix();
	
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
static int evalExpression(const std::string &expr, double value)
{
	a_assert(expr, value);
	std::cout << "------------------------------" << std::endl;
	return 0;
} /* eval_expression */

/*----------------------------------------------*/
int main(void)
{
	evalExpression("+14.0", 14);
	evalExpression("-14.0", -14);
	evalExpression("+14", 14);
	evalExpression("7+7", 14);
	evalExpression("-1+2", 1);
	evalExpression("(5+10*(3-9))", -55);
        evalExpression(".0 + .1 + .2 + .3 + .5 + .6", 1.7);
	evalExpression("1.0+3.0*2.0",1.0+3.0*2.0); 
	evalExpression("1.0-3.0*2.0",1.0-3.0*2.0); 
	evalExpression("1.0 *                           3.0+2.0", 5);
	evalExpression("-4.0-8.1", -4.0-8.1);
	evalExpression("1.0*3.0-2.0", 1.0*3.0-2.0);
	evalExpression("1.0+3.0/2.0", 1.0+3.0/2.0);
	evalExpression("1.0+(3.0*2.0-4.0)", 1.0+(3.0*2.0-4.0));
	evalExpression("1.0-(3.0*2.0+4.0)", 1.0-(3.0*2.0+4.0));
	evalExpression("2*3**2)", 2*pow(3,2));
	evalExpression("2**3**2)", 512);
	evalExpression("2 ** (-1)", pow(2, -1));
	evalExpression("2 ** (-0.2 + 0.2)", pow(2, -0.2+0.2));
	evalExpression("0**2", pow(0, 2));

	std::cout << "ALL TESTS PASSED" << std::endl;
	return 0;

} /* main */

/*----------------------------------------------*/
