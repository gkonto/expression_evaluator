#include <vector>
#include <list>
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <assert.h>
#include <iomanip>

#include "evaluator.hpp"

#define _USE_MATH_DEFINES

static std::string doubleToString(const double value)
{
	//FIXME ti kano edo ? poso prepei na einai to precision????
	std::ostringstream ss;
	ss << std::setprecision(10) << value;
	return ss.str();
} /* doubleToString */

static double stringToDouble(const std::string &value)
{
	return atof(value.c_str());
} /* stringToDouble */

static Token subTokens(const Token &left, const Token &right)
{
	//TODO simazepse to xaos kai stin add
	Token new_tok;
	double d_left = stringToDouble(left.value);
	double d_right = stringToDouble(right.value);
	double value = d_left - d_right;

	new_tok.type  = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
} /* subTokens */

static Token addTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = stringToDouble(left.value.c_str());
	double d_right = stringToDouble(right.value.c_str());
	double value = d_left + d_right;

	new_tok.type  = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
} /* addTokens */

static Token mulTokens(const Token &left, const Token &right)
{
	//TODO make fun
	Token new_tok;
	double d_left = stringToDouble(left.value.c_str());
	double d_right = stringToDouble(right.value.c_str());
	double value = d_left * d_right;

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
} /* mulTokens */

static Token divTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = stringToDouble(left.value.c_str());
	double d_right = stringToDouble(right.value.c_str());
	double value = d_left / d_right;

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
} /* divTokens */

static Token powTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = stringToDouble(left.value.c_str());
	double d_right = stringToDouble(right.value.c_str());
	double value = pow(d_left, d_right);

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
} /* powTokens */


static Token calcBinaryExpr(const Token &left, const Token &right, const Token &operat)
{
	Token t;

	if (operat.type ==  Token::E_ADD) {
		t = addTokens(left, right);
		return t;
	} else if (operat.type == Token::E_SUB) {
		t = subTokens(left, right);
		return t;
	} else if (operat.type == Token::E_MUL) {
		t = mulTokens(left, right);
		return t;
	} else if (operat.type == Token::E_DIV) {
		t = divTokens(left, right);
		return t;
	} else if (operat.type == Token::E_POW) {
		t = powTokens(left, right);
		return t;
	} else {
		t.type = Token::E_ERROR;
		std::cout << "Should not reach this place" << std::endl;
		assert(t.type == Token::E_ERROR);
		return t;
	}
} /* calcBinaryExpr */

static Token calcFunExpr(const Token &fun, const Token &tok)
{
	Token t;
	//TODO strcmp??
	//tolower
	//find a way to represent all funs
	if (fun.value == "cos") {
		t.type = Token::E_NUMBER;
		double value = stringToDouble(tok.value);
		t.value = doubleToString(cos(value));

	} else if (fun.value == "sin") {
		t.type = Token::E_NUMBER;
		double value = stringToDouble(tok.value);

		t.value = doubleToString(sin(value));

	} else if (fun.value == "log") {
		t.type = Token::E_NUMBER;
		double value = stringToDouble(tok.value);
		t.value = doubleToString(log(value));
	} else {
		t.type = Token::E_ERROR;
		std::cout << "Should not be here calcFunExpr" << std::endl;
		assert(t.type == Token::E_ERROR);
	}
	return t;
}

void Evaluator::displayStack()
{
	std::cout << "stack_: " << std::endl;

	for (size_t i = 0; i < stack_.size(); i++)
	{
		std::cout << stack_[i].value << " ";
	}
	std::cout << std::endl;
} /* Evaluator::displayStack */

std::vector<Token> Evaluator::evaluate()
{
	std::cout << "Entered evaluate phase..." << std::endl;
	while(!postfix_.empty()) {

		Token tmp = postfix_.front();
	       	postfix_.pop_front();
		if (tmp.type == Token::E_NUMBER) {
			stack_.push_back(tmp);
		} else if (tmp.type == Token::E_ADD ||
		   	tmp.type == Token::E_SUB    ||
	       		tmp.type == Token::E_MUL    ||
			tmp.type == Token::E_DIV    ||
			tmp.type == Token::E_POW)
		{
			Token right = stack_.back();
			stack_.pop_back();
			Token left ;
			if (!stack_.empty()) {
				left  = stack_.back();
				stack_.pop_back();
			} else {
				left.type = Token::E_NUMBER;
				left.value = '0';
			}
			Token new_val = calcBinaryExpr(left, right, tmp);
			stack_.push_back(new_val);
		} else if (tmp.type == Token::E_SYMBOL && (tmp.value == "cos" || tmp.value == "sin" || tmp.value == "log")) {
			//TODO de maresei edo. ti tha ginei an pairnei perissotera tokens apo ena ?
			//TODO pos tha ginei na diorthoso ena terastio loop
			Token next_tok = stack_.back();
			stack_.pop_back();

			Token new_val = calcFunExpr(tmp, next_tok);
			stack_.push_back(new_val);
		}

		displayStack();
/*		for (int i = 0; i < stack_.size() ; i++)*/
/*		{*/
/*			std::cout << stack_[i].value << std::endl;*/
/*		}*/
	}
	return stack_;
} /* Evaluator::evaluate */
