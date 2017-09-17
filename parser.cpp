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
	int token_precedence = tok.getPrecedence(tok.type);
	int stack_precedence = tok.getPrecedence(stack_.back().type);

	if (token_precedence < stack_precedence ||
	   	token_precedence == stack_precedence) 
	{
		return true;
	}
	return false;

} /* Parser::isStackTokenHigherOrEqualPrecedence */

bool Parser::isStackEqualPrecedence(Token &tok)
{
	int token_precedence = tok.getPrecedence(tok.type);
	int stack_precedence = tok.getPrecedence(tok.type);

	if (token_precedence == stack_precedence)
       	{
		return true;
	}
	return false;
} /* Parser::isStackEqualPrecedence */

void Parser::addOperator(Token &tok)
{
	while (!stack_.empty() &&
	      	tok.isLeftAssociative(tok) &&
	       	isStackTokenHigherOrEqualPrecedence(tok))
       	{
		//case 3-+-5 etc...
/*		if (tok.getPrecedence(tok.type) == 2 && isStackEqualPrecedence(tok)) {*/
/*			std::cout << "MPIKEEEEEEEEEEEE" << std::endl;*/
/*			if (tok.type != stack_.back().type) {*/
/*				stack_.pop_back();*/
/*				tok.type = Token::E_SUB;*/
/*				tok.value = '-';*/
/*			} else {*/
/*				stack_.pop_back();*/
/*				tok.type = Token::E_ADD;*/
/*				tok.value = '+';*/
/*			}*/
/*		} else {*/
			postfix_.push_back(stack_.back());
			stack_.pop_back();
/*		}*/
	}
	stack_.push_back(tok);
} /* Parser::addOperator */

bool Parser::frontStackIsLeftBracket()
{
	#ifdef DBG
		std::cout << "Parser::frontStackIsLeftBracket" << std::endl;
		std::cout << (stack_.back()).isLeftBracket(stack_.back()) << std::endl;
	#endif
	return (stack_.back()).isLeftBracket(stack_.back());
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
		#ifdef DBG
			std::cout << "Entered loop" << std::endl;
		#endif
		Token tok = token_list_[i];
		if (tok.isNumber(tok) ) {
			postfix_.push_back(tok);
		} else if (tok.isOperator(tok)) {
			addOperator(tok);
		} else if ((stack_.back()).isLeftBracket(tok) || isFun(tok)) {

			stack_.push_back(tok);
		} else if (tok.isRightBracket(tok)) {
			#ifdef DBG
				std::cout << "entered WHILE" << std::endl;
			#endif
			while (!stack_.empty() &&
				!frontStackIsLeftBracket())
			{
				postfix_.push_back(stack_.back());
				safePopBack<Token>(stack_);

				#ifdef DBG
					std::cout << "JUST OUT OF STACK" << std::endl;
				#endif

			}

			#ifdef DBG
				std::cout << "ENDED while" << std::endl;
			#endif

			if (frontStackIsLeftBracket()) {
				safePopBack<Token>(stack_);
				#ifdef DBG
					std::cout << "JUST AFTER while" << std::endl;
				#endif

				if (!stack_.empty() && isFun(stack_.back())) {

					#ifdef DBG
						std::cout << "isfun" << std::endl;
					#endif
					postfix_.push_back(stack_.back());
					safePopBack<Token>(stack_);
				}
			}

			#ifdef DBG
				std::cout << "to new loop" << std::endl;
			#endif
		}
		#ifdef DBG
			std::cout << "ABOUT TO FINISH " << std::endl;
		#endif

		if (SHOW_DETAILED_CALCULATION) {
			displayCurrentState();
		}
	}
	#ifdef DBG
		std::cout << "Out of For LOOP" << std::endl;
	#endif

	while (!stack_.empty()) {
		postfix_.push_back(stack_.back());
		stack_.pop_back();
	}
	if (SHOW_DETAILED_CALCULATION) {
		displayCurrentState();
	}
} /* Parser::shuntingYard */
