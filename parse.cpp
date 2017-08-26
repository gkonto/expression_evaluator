#include "parse.hpp"



void Parser::display_current_state()
{
	std::size_t i = 0;

	std::cout << "Stack   | ";
	for (i = 0; i < stack_.size(); i++) {
		std::cout << stack_[i].value;
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "PostFix | ";
	std::list<Token>::iterator iter;
	for ( iter = postfix_.begin(); iter != postfix_.end(); iter++) {
		std::cout << iter->value ;
		std::cout << " " ;
	}
	std::cout << std::endl << std::endl;;
}

void Parser::display_token_vector()
{
	std::size_t i = 0;
	for (i = 0; i < token_list_.size(); i++) 
	{
		std::cout << token_list_[i].value << " ";
	}

	std::cout << std::endl;
}

bool Parser::is_number(const Token &tok)
{
	return (tok.type == Token::E_NUMBER);
}

bool Parser::is_operator(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD ||
		tok.type == Token::E_POW );
}

bool Parser::is_left_associative(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD );
} /* Parser::is_left_associative */

int Parser::get_precedence(Token::token_type tt)
{
	if ( tt == Token::E_ADD || tt == Token::E_SUB) {
		return 2;
	} else if ( tt == Token::E_MUL || tt == Token::E_DIV || tt == Token::E_MOD) {
		return 3;
	} else if (tt == Token::E_POW) {
		return 4;	
	} else {
		return 0;
	}
} /* Parser::get_precedence */

bool Parser::is_left_bracket(const Token &tok)
{
	return (tok.type == Token::E_LBRACKET);
} /* Parser::is_left_bracket */

bool Parser::is_right_bracket(const Token &tok)
{
	return (tok.type == Token::E_RBRACKET);
} /* Parser::is_left_bracket */

bool Parser::is_stack_token_higher_or_equal_precedence(const Token &tok)
{
	int token_precedence = get_precedence(tok.type);
	int stack_precedence = get_precedence(stack_.back().type);

	if (token_precedence < stack_precedence ||
	   	token_precedence == stack_precedence) 
	{
		return true;
	}
	return false;

} /* Parser::is_higher_or_equal_precedence */

void Parser::add_operator(const Token &tok)
{
	while (!stack_.empty() &&
	      	is_left_associative(tok) &&
	       	is_stack_token_higher_or_equal_precedence(tok))
       	{
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	stack_.push_back(tok);
} /* Parser::add_operator */

bool Parser::front_stack_is_left_bracket()
{
	return is_left_bracket(stack_.back());
}


void Parser::shunting_yard()
{
/*
 * Implementation Details:
 * while there are tokens to be read:
 * 	read a token
 * 	if the token is a number, them push it to the postfix_
 * 	if the token is an operator, then:
 * 		while there is an operator at the top of the operator stach with
 * 		greater than or equal to precedence and the operator is left associative:
 * 			pop operators from the operator stack, onto the output queue.
 * 			push the read operator onto the operator stack.
 * 	if the token is a left bracket ( i.e "("), then:
 * 		push it onto the operator stack.
 * 	if the token is a right brack(i.e ")", then:
 * 		while the operator at the top of the operator stack is not a left bracket:
 * 			pop operators from the operator stack onto the output queue.
 * 		pop th eleft bracket from the stack
 * if there are no more tokens to read:
 * 	while there are still operator tokens on the stack:
 * 		pop the operator onto the output queue.
 */
	std::cout << "PARSING phase..." << std::endl;
	display_token_vector();
	for (std::size_t i = 0; i < token_list_.size(); i++)
	{
		Token tok = token_list_[i];
		if (is_number(tok)) {
			postfix_.push_back(tok);
		} else if (is_operator(tok)) {
			add_operator(tok);
		} else if (is_left_bracket(tok)) {
			stack_.push_back(tok);
		} else if (is_right_bracket(tok)) {
			while (!stack_.empty() &&
				!front_stack_is_left_bracket())
			{
				postfix_.push_back(stack_.back());
				stack_.pop_back();
			}
			if (front_stack_is_left_bracket()) {
				stack_.pop_back();
			}
		}

		display_current_state();
	}

	while (!stack_.empty()) {
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	display_current_state();
}
