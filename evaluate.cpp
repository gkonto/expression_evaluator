#include <vector>
#include <list>
#include <string.h>
#include "evaluate.hpp"
#include <iostream>
#include <math.h>
#include <sstream>

static std::string doubleToString(const double value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

static double stringToDouble(const std::string &value)
{
	return atof(value.c_str());
}

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
}

static Token addTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = atof(left.value.c_str());
	double d_right = atof(right.value.c_str());
	double value = d_left + d_right;

	new_tok.type  = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
}

static Token mulTokens(const Token &left, const Token &right)
{
	//TODO make fun
	Token new_tok;
	double d_left = atof(left.value.c_str());
	double d_right = atof(right.value.c_str());
	double value = d_left * d_right;

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
}

static Token divTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = atof(left.value.c_str());
	double d_right = atof(right.value.c_str());
	double value = d_left / d_right;

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
}

static Token powTokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = atof(left.value.c_str());
	double d_right = atof(right.value.c_str());
	double value = pow(d_left, d_right);

	new_tok.type = Token::E_NUMBER;
	new_tok.value = doubleToString(value);
	return new_tok;
}


static Token calc(const Token &left, const Token &right, const Token &operat)
{
	if (operat.type ==  Token::E_ADD) {
		return addTokens(left, right);
	} else if (operat.type == Token::E_SUB) {
		return subTokens(left, right);
	} else if (operat.type == Token::E_MUL) {
		return mulTokens(left, right);
	} else if (operat.type == Token::E_DIV) {
		return divTokens(left, right);
	} else if (operat.type == Token::E_POW) {
		return powTokens(left, right);
	}
}

void Evaluator::displayStack()
{
	std::cout << "stack_: " << std::endl;

	for (size_t i = 0; i < stack_.size(); i++)
	{
		std::cout << stack_[i].value << " ";
	}
	std::cout << std::endl;
}

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
			Token new_val = calc(left, right, tmp);
			stack_.push_back(new_val);
		}
		displayStack();
/*		for (int i = 0; i < stack_.size() ; i++)*/
/*		{*/
/*			std::cout << stack_[i].value << std::endl;*/
/*		}*/
	}
	return stack_;
}
