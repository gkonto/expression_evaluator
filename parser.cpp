#include "parser.hpp"
#include "tools.hpp"

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
} /* Parser::displayCurrentState */

void Parser::displayTokenVector()
{
	std::size_t i = 0;
	for (i = 0; i < token_list_.size(); i++) 
	{
		std::cout << token_list_[i].value << " ";
	}

	std::cout << std::endl;
} /* Parser::displayTokenVector */

bool Parser::isNumber(const Token &tok)
{
	return (tok.type == Token::E_NUMBER);
} /* Parser::isNumber */

bool Parser::isOperator(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD ||
		tok.type == Token::E_POW );
} /* Parser::isOperator */

bool Parser::isLeftAssociative(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD );
} /* Parser::isLeftAssociative */

int Parser::getPrecedence(Token::token_type tt)
{
	//TODO save in the Token Struct the precedence !
	if ( tt == Token::E_ADD || tt == Token::E_SUB) {
		return 2;
	} else if ( tt == Token::E_MUL || tt == Token::E_DIV || tt == Token::E_MOD) {
		return 3;
	} else if (tt == Token::E_POW) {
		return 4;	
	} else {
		return 0;
	}
} /* Parser::getPrecedence */

bool Parser::isLeftBracket(const Token &tok)
{
	return (tok.type == Token::E_LBRACKET);
} /* Parser::isLeftBracket */

bool Parser::isRightBracket(const Token &tok)
{
	return (tok.type == Token::E_RBRACKET);
} /* Parser::isRightBracket */

bool Parser::isFun(const Token &tok)
{
	//TODO to lower
	//strcmp??
	if (tok.type == Token::E_SYMBOL) {
		return (tok.value == "cos" ||
			tok.value == "sin" ||
			tok.value == "log");
	} else {
		return false;
	}
}


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

} /* Parser::isStackTokenHigherOrEqualPrecedence */

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
} /* Parser::addOperator */

bool Parser::frontStackIsLeftBracket()
{
	return isLeftBracket(stack_.back());
} /* Parser::frontStackIsLeftBracket */


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
	if (SHOW_DETAILED_CALCULATION) {
		std::cout << "PARSING phase..." << std::endl;
		displayTokenVector();
	}
	for (std::size_t i = 0; i < token_list_.size(); i++)
	{
		Token tok = token_list_[i];
		if (isNumber(tok) ) {
			postfix_.push_back(tok);
		} else if (isOperator(tok)) {
			addOperator(tok);
		} else if (isLeftBracket(tok) || isFun(tok)) {
			stack_.push_back(tok);
		} else if (isRightBracket(tok)) {

			while (!stack_.empty() &&
				!frontStackIsLeftBracket())
			{
				postfix_.push_back(stack_.back());
				safePopBack<Token>(stack_);
			}

			if (frontStackIsLeftBracket()) {
				safePopBack<Token>(stack_);
				if (isFun(stack_.back())) {
					postfix_.push_back(stack_.back());
					safePopBack<Token>(stack_);
				}
			}

		}

		if (SHOW_DETAILED_CALCULATION) {
			displayCurrentState();
		}
	}

	while (!stack_.empty()) {
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	if (SHOW_DETAILED_CALCULATION) {
		displayCurrentState();
	}
} /* Parser::shuntingYard */
