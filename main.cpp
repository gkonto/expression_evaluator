#include <string>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "tools.hpp"
/*----------------------------------------------*/

bool SHOW_DETAILED_CALCULATION;
bool RUN_TEST_EXPRESSIONS ;
int FAILED_EXPRESSIONS;
int PASSED_EXPRESSIONS;


static double evaluate(const std::string &expr)
{
	//lexer
	Lexer lex;
	lex.process(expr);
	if (SHOW_DETAILED_CALCULATION) {
		lex.display();
	}
	std::vector<Token> token_list = lex.getTokens();
	
	//parse
	Parser parse(token_list);
	parse.shuntingYard();
	std::list<Token> postfix = parse.getPostFix();
	
	//evaluator
	Evaluator eval(postfix);
	std::vector<Token> stack_ = eval.evaluate();
	double value = 0;
	if (!stack_.empty()) {
		value = atof(stack_[0].value.c_str());
	} else {
		std::cout << "STACK IS EMPTY, INVALID EXPRESSION!" << std::endl;
	}

	if (SHOW_DETAILED_CALCULATION) {
		std::cout << std::setprecision(20) <<"Calculated Number is: " << value << std::endl;
/*		std::cout << "Calculated Number is: " << value << std::endl;*/
	}
	return value;
} /* evaluate */

/*----------------------------------------------*/

/*----------------------------------------------*/
static int isEqual(const std::string &expr, double value)
{
	double eval = evaluate(expr);

	if (SHOW_DETAILED_CALCULATION) {
		std::cout << std::setprecision(20) << "Comparing expr : " << eval << " with value : " << value << std::endl;
	}

	if (!doubleEquals(eval, value)) {

		if (SHOW_DETAILED_CALCULATION) {
			std::cout << "Test Failed in expression: " << expr << std::endl;
		}
/*		assert(eval == value);*/
		return 1;
	}
	return 0;
} /* isEqual */

/*----------------------------------------------*/

/*----------------------------------------------*/
static int evalExpression(const std::string &expr, double value)
{
	int ret = isEqual(expr, value);
	if (SHOW_DETAILED_CALCULATION) {
		std::cout << "------------------------------" << std::endl;
	}
	return ret;
} /* eval_expression */

/*----------------------------------------------*/

/*----------------------------------------------*/
static int getFileWithRandomExpression(const char *argv, std::ifstream &file )
{
	file.open(argv, std::ifstream::in);

	if (file.is_open()) {
		return E_NONE;
	} else {
		return E_INVALID_ARG;
	}
	return E_NONE;
}

/*----------------------------------------------*/

/*----------------------------------------------*/
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

/*----------------------------------------------*/

/*----------------------------------------------*/
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

/*----------------------------------------------*/

/*----------------------------------------------*/
static void showHelpSummary() {
	std::cout <<"\t--help\t\t      : shows the available arguments with description" <<std::endl << std::endl;
	std::cout <<"\t-f [filename]\t      : This argument followed by a file (full file's path)\n\
			        can calcualte all the expression of the file.\n\
			        The file (.txt) should have the following format:\n\
			        (line1): 'expression1'\t'value'\n\
			        (line2): 'expression2'\t'value'" <<std::endl << std::endl; 
	std::cout <<"\t -d [no, yes]\t      : Show the detailed calculation from the beginning to the end. (default->yes) [NOT WORKING YET]" << std::endl << std::endl;
	std::cout <<"\t -t [run, do_not_run] : Runs the built in expressions (just for testing). (default : run)" << std::endl << std::endl;
}

/*----------------------------------------------*/

/*----------------------------------------------*/
static int parseArgs(const int argc, char **argv, std::ifstream &file)
{
	int err_type = E_NONE;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--help")) {
			showHelpSummary();
			return  JUST_SHOW_HELP;
		}

	}

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

/*----------------------------------------------*/

/*----------------------------------------------*/
bool isParseError(int err_type)
{
	switch (err_type) {
		case E_NONE:
			return false;
		case E_INVALID_ARG:
			std::cout << "Invalid arguments given!" << std::endl;
			return true;
		case JUST_SHOW_HELP:
			return true;
		default:
			assert(true);
	}
	return false;
}

/*----------------------------------------------*/

/*----------------------------------------------*/
static void runDefaultTests()
{
	evalExpression("", 0);
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
}

/*----------------------------------------------*/

/*----------------------------------------------*/
static void runTestsFromFile(std::ifstream &file)
{
	std::string line;
	std::string expression;
	double value = 0;
	std::fstream failed;
	failed.open("failed.txt", std::fstream::in | std::fstream::out | std::fstream::app );

	while (std::getline(file, line))
	{
		#ifdef DBG	
			std::cout << "Line : " << PASSED_EXPRESSIONS + FAILED_EXPRESSIONS + 1 << std::endl;
		#endif
		std::istringstream iss(line);
		if (!(iss>>expression>>value)) {
			std::cout << "Invalid line found in line number: "<< FAILED_EXPRESSIONS + PASSED_EXPRESSIONS+1 << std::endl;
		} else {
			int ret = evalExpression(expression, value);
			if (ret == 1) {
				failed << expression << "\t\t\t" << value << std::endl;
				FAILED_EXPRESSIONS++;
			} else {
				PASSED_EXPRESSIONS++;
			}

		}

	}
}

/*----------------------------------------------*/

/*----------------------------------------------*/
int main(int argc, char **argv)
{

	std::ifstream file;
	int err_type = E_NONE;
	SHOW_DETAILED_CALCULATION = true;
	RUN_TEST_EXPRESSIONS = false;
	FAILED_EXPRESSIONS = 0;
	PASSED_EXPRESSIONS = 0;
	clock_t begin = clock();

	err_type = parseArgs(argc, argv, file);

	if (isParseError(err_type))
       	{
		return 1;
	}

	if (RUN_TEST_EXPRESSIONS)
       	{
		runDefaultTests();
	}

	if (file.is_open())
	{
		runTestsFromFile(file);
	}

	std::cout << "Tested " << PASSED_EXPRESSIONS + FAILED_EXPRESSIONS << " expressions" << std::endl;
	std::cout << "PASSED " << PASSED_EXPRESSIONS << std::endl;
	clock_t end = clock();
	std::cout << double(end -begin) / CLOCKS_PER_SEC << std::endl;

	return 0;

} /* main */

/*----------------------------------------------*/
