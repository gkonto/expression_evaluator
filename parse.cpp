#include "parse.hpp"


void Parser::displayCurrentState()
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

void Parser::displayTokenVector()
{
	std::size_t i = 0;
	for (i = 0; i < token_list_.size(); i++) 
	{
		std::cout << token_list_[i].value << " ";
	}

	std::cout << std::endl;
}

bool Parser::isNumber(const Token &tok)
{
	return (tok.type == Token::E_NUMBER);
}

bool Parser::isOperator(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD ||
		tok.type == Token::E_POW );
}

bool Parser::isLeftAssociative(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD );
} /* Parser::is_left_associative */

int Parser::getPrecedence(Token::token_type tt)
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

bool Parser::isLeftBracket(const Token &tok)
{
	return (tok.type == Token::E_LBRACKET);
} /* Parser::is_left_bracket */

bool Parser::isRightBracket(const Token &tok)
{
	return (tok.type == Token::E_RBRACKET);
} /* Parser::is_left_bracket */

bool Parser::isStackTokenHigherOrEqualPrecedence(const Token &tok)
{
	int token_precedence = getPrecedence(tok.type);
	int stack_precedence = getPrecedence(stack_.back().type);

	if (token_precedence < stack_precedence ||
	   	token_precedence == stack_precedence) 
	{
		return true;
	}
	return false;

} /* Parser::is_higher_or_equal_precedence */

void Parser::addOperator(const Token &tok)
{
	while (!stack_.empty() &&
	      	isLeftAssociative(tok) &&
	       	isStackTokenHigherOrEqualPrecedence(tok))
       	{
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	stack_.push_back(tok);
} /* Parser::add_operator */

bool Parser::frontStackIsLeftBracket()
{
	return isLeftBracket(stack_.back());
}


void Parser::shuntingYard()
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
	displayTokenVector();
	for (std::size_t i = 0; i < token_list_.size(); i++)
	{
		Token tok = token_list_[i];
		if (isNumber(tok)) {
			postfix_.push_back(tok);
		} else if (isOperator(tok)) {
			addOperator(tok);
		} else if (isLeftBracket(tok)) {
			stack_.push_back(tok);
		} else if (isRightBracket(tok)) {
			while (!stack_.empty() &&
				!frontStackIsLeftBracket())
			{
				postfix_.push_back(stack_.back());
				stack_.pop_back();
			}
			if (frontStackIsLeftBracket()) {
				stack_.pop_back();
			}
		}

		displayCurrentState();
	}

	while (!stack_.empty()) {
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	displayCurrentState();
}
