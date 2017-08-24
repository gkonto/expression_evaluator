#include <vector>
#include <list>
#include <string.h>
#include "evaluate.hpp"
#include <iostream>
#include <sstream>

static std::string double_to_string(const double value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

static double string_to_double(const std::string &value)
{
	return atof(value.c_str());
}

static Token sub_tokens(const Token &left, const Token &right)
{
	//TODO simazepse to xaos kai stin add
	Token new_tok;
	double d_left = string_to_double(left.value);
	double d_right = string_to_double(right.value);
	double value = d_left - d_right;

	new_tok.type  = Token::E_NUMBER;
	new_tok.value = double_to_string(value);
	return new_tok;
}

static Token add_tokens(const Token &left, const Token &right)
{
	Token new_tok;
	double d_left = atof(left.value.c_str());
	double d_right = atof(right.value.c_str());
	double value = d_left + d_right;

	new_tok.type  = Token::E_NUMBER;
	new_tok.value = double_to_string(value);
	return new_tok;
}

static Token calc(const Token &left, const Token &right, const Token &operat)
{

	if (operat.type ==  Token::E_ADD) {
		return add_tokens(left, right);
	} else if (operat.type == Token::E_SUB) {
		return sub_tokens(left, right);
	}
}

std::vector<Token> Evaluator::evaluate()
{
	std::cout << "Entered evaluate phase..." << std::endl;
	while(!postfix_.empty()) {

		Token tmp = postfix_.front();
	       	postfix_.pop_front();
		if (tmp.type == Token::E_NUMBER) {
			stack_.push_back(tmp);
		} else if (tmp.type == Token::E_ADD || tmp.type == Token::E_SUB) {
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
/*		for (int i = 0; i < stack_.size() ; i++)*/
/*		{*/
/*			std::cout << stack_[i].value << std::endl;*/
/*		}*/
	}
	return stack_;
}
