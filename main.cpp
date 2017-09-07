#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
/*----------------------------------------------*/
bool SHOW_DETAILED_CALCULATION = true;
bool RUN_TEST_EXPRESSIONS = false;

static bool doubleEquals(const double a, const double  b, const double epsilon = 0.001)
{
	return ((abs(a)-abs(b)) < epsilon);
}


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

	if (!doubleEquals(eval, value)) {
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

/*----------------------------------------------*/
typedef enum parseErrorTypes {
	E_NONE = 0,
	E_INVALID_ARG,
	E_NO_ARG_GIVEN
} parseErrorTypes;

/*----------------------------------------------*/

/*----------------------------------------------*/
static int getFileWithRandomExpression(const char *argv, std::ifstream &file )
{
	file.open(argv, std::ifstream::in);
	std::cout << "getFileWithRandomExpression 1" << std::endl;

	if (file.is_open()) {
		std::cout << "getFileWithRandomExpression 2" << std::endl;
		return E_NONE;
	} else {
		std::cout << "getFileWithRandomExpression 3" << std::endl;
		return E_INVALID_ARG;
	}
	return E_NONE;
}

static int configureDetailedInfo(const char *argv)
{
	if (!strcmp(argv, "no")) {
		SHOW_DETAILED_CALCULATION = false;
	} else if (!strcmp(argv, "yes")) {
		SHOW_DETAILED_CALCULATION = true;
	} else {
		return E_INVALID_ARG;
	}
	return E_NONE;
}

static int configureRunTestExpressions(const char *argv)
{
	if (!strcmp(argv, "run")) {
		RUN_TEST_EXPRESSIONS = true;
	} else if (!strcmp(argv, "do_not_run")) {
		RUN_TEST_EXPRESSIONS = false;
	} else {
		return E_INVALID_ARG;
	}
	return E_NONE;
}

static int parseArgs(const int argc, char **argv, std::ifstream &file)
{
	int err_type = E_NONE;

	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-f")) {
			if (argv[i+1]) {
				err_type = getFileWithRandomExpression(argv[i+1], file);
			} else {
				err_type = E_INVALID_ARG;

			}
		} else if (!strcmp(argv[i], "-d")) {
			if (argv[i+1]) {
				err_type = configureDetailedInfo(argv[i+1]);
			} else {
				err_type = E_INVALID_ARG;
			}
		} else if (!strcmp(argv[i], "-t")) {
			if (argv[i+1]) {
				err_type = configureRunTestExpressions(argv[i+1]);
			} else {
				err_type = E_INVALID_ARG;
			}
		}
		if (err_type != E_NONE) {
			return err_type;
		}
	}
	return err_type;
}

bool isParseError(int err_type)
{
	switch (err_type) {
		case E_NONE:
			return false;
		case E_INVALID_ARG:
			std::cout << "Invalid arguments given!" << std::endl;
			return true;
		default:
			assert(true);
	}
	return false;
}

static int runDefaultTests()
{
//If expression added, update the counter!
	int counter = 20;

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

	return counter;
}

static void runTestsFromFile(std::ifstream &file, int &counter)
{
	std::string line;
	std::string expression;
	double value = 0;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		if (!(iss>>expression>>value)) {
			std::cout << "Invalid line found in line number: "<< counter+1 << std::endl;
		} else {
			evalExpression(expression, value);
		}

		counter++;
	}
}
/*----------------------------------------------*/

/*----------------------------------------------*/
int main(int argc, char **argv)
{

	std::ifstream file;
	int err_type = E_NONE;
	int counter = 0;	

	err_type = parseArgs(argc, argv, file);

	if (isParseError(err_type))
       	{
		return 1;
	}

	if (RUN_TEST_EXPRESSIONS)
       	{
		counter = runDefaultTests();
	}

	if (file.is_open())
	{
		runTestsFromFile(file, counter);
	}

	std::cout << "Tested " << counter << " expressions" << std::endl;
	std::cout << "ALL TESTS PASSED" << std::endl;

	return 0;

} /* main */

/*----------------------------------------------*/
