#include "parser.hpp"
#include "tools.hpp"
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <stack>

void buildOpCore(Node *binary, std::vector<Node *>&nodes)
{
	if (!nodes.empty()) {
		binary->setRhs(nodes.back());
		nodes.pop_back();
	}
	if (!nodes.empty()) {
		binary->setLhs(nodes.back());
		nodes.pop_back();
	}
	nodes.push_back(binary);
}

void SubOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* SubOp::build */

void AddOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* AddOp::build */

void MulOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* MulOp::build */

void DivOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* DivOp::build */

void ModOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* ModOp::build */

void PowOp::build(std::vector<Node *> &nodes)
{
	buildOpCore(this, nodes);
} /* PowOp::build */

Token Node::getToken()
{
	Token tok;
	return tok;
}

void ExpressionNode::build(std::vector<Token> &tokens)
{
	std::list<Token> postfix = Parser::shuntingYard(tokens);
	std::cout << "Finished ShuntingYard" << std::endl;
	
	std::vector<Node *> stack;
	std::list<Token>::const_iterator it;
	for (it = postfix.begin(); it != postfix.end(); it++)
	{
		Node *node = Parser::createNode(*it);
		if (Token::isNumber(*it))
	       	{
			stack.push_back(node);
		}
		node->setToken(*it);
		node->build(stack);
	}	
	//Prepei to stack edo na einai panta size() = 1 !
	//an den einai na bgalo assert! kai na elegxo se poies
	//periptoseis to stack mporei na einai > 1.
	if (stack.size() != 1) {
		std::cout << "Parser.cpp: ExpressionNode::build" << std::endl;
		std::cout << "TO STACK THeORITIKA PREPEI NA EINAI 1 edo !" << std::endl;
	} else {
		this->e_node = stack[0];
	}
}

double ExpressionNode::eval()
{
	if (this->e_node) {
		return (this->e_node->eval());
	}
	return 0;
}

double SubOp::eval()
{
	double left = 0;
	if (this->lhs_) {
		left =  (lhs_->eval());
	}
	double right = 0;
	if (this->rhs_) {
		right = (rhs_->eval());
	}
	return (left-right);
}

double AddOp::eval()
{
	double left = 0;
	if (this->lhs_) {
		left = (lhs_->eval());
	}
	double right = 0;
	if (this->rhs_) {
		right = (rhs_->eval());
	}
	return (left+right);
}

double MulOp::eval()
{
	double left = 0;
	if (this->lhs_) {
		left = (lhs_->eval());
	}
	double right = 0;
	if (this->rhs_) {
		right = (rhs_->eval());
	}
	return (left*right);
}

double DivOp::eval()
{
	double left = 0;
	if (this->lhs_) {
		left = (lhs_->eval());
	}
	double right = 0;
	if (this->rhs_) {
		right = (rhs_->eval());
	}
	return (left/right);
}

/*double ModOp::eval()*/
/*{*/
/*	double left = 0;*/
/*	if (this->lhs_) {*/
/*		left = (lhs_->eval());*/
/*	}*/
/*	double right = 0;*/
/*	if (this->rhs_) {*/
/*		right = (rhs_->eval());*/
/*	}*/
/*	return (left % right);*/
/*}*/

double PowOp::eval()
{
	double left = 0;
	if (this->lhs_) {
		left = (lhs_->eval());
	}
	double right = 0;
	if (this->rhs_) {
		right = (rhs_->eval());
	}
	return pow(left,right);
}

double Number::eval()
{
	return atof(this->getToken().value.c_str());
}

void BracketChecker::reset()
{
	stack_ = std::stack<char>();
	state_ = true;
	errorToken_.clear();
}

std::size_t BracketChecker::checkBracketValidity(std::vector<Token> &tokens)
{
	if (!tokens.empty()) {
		for (std::size_t i = 0; i < (tokens.size()); ++i) {
			const Token &t0 = tokens[i];
			std::cout << tokens[i].value << std::endl;
			if (!operator()(t0)) {
				return i;
			}
		}
	}
	return tokens.size();
}

bool BracketChecker::operator()(const Token &t)
{
	if (!t.value.empty() && (Token::E_STRING != t.type)
		&& (Token::E_SYMBOL != t.type) && (details::isBracket(t.value[0]))) {
		char c = t.value[0];

		if (t.type == Token::E_LBRACKET) {
			stack_.push(')');
		} else if (t.type == Token::E_LCRLBRACKET) {
			stack_.push('}');
		} else if (t.type == Token::E_LSQRBRACKET) {
			stack_.push(']');
		} else if (details::isRightBracket(c)) {
			if (stack_.empty()) {
				state_ = false;
				errorToken_ = t;
				return false;
			} else if (c != stack_.top()) {
				state_ = false;
				errorToken_ = t;
				return false;
			} else {
				stack_.pop();
			}
		}
	}
	return true;
}

Node *Parser::createNode(const Token &tok)
{
	if (Token::isNumber(tok)) {
		Number *node = new Number;
		return node;
	} else if (Token::isOperator(tok)) {
		if (Token::isSub(tok)) {
			SubOp *node = new SubOp;
			return node;
		} else if (Token::isAdd(tok)) {
			AddOp *node = new AddOp;
			return node;
		} else if (Token::isMul(tok)) {
			MulOp *node = new MulOp;
			return node;
		} else if (Token::isDiv(tok)) {
			DivOp *node = new DivOp;
			return node;
		} else if (Token::isMod(tok)) {
			ModOp *node = new ModOp;
			return node;
		} else if (Token::isPow(tok)) {
			PowOp *node = new PowOp;
			return node;
		} else {
			return NULL;
		}
	} 
	return NULL;
}

Node *Parser::createNode(std::vector<Token> &tokens)
{
	std::vector<Token>::iterator it;

	// else it is expression, assignment etc.
	if (Token::isAssign(tokens[1])) { //maybe its an assignment

	} else {
		ExpressionNode *node = new ExpressionNode;
		return node;
	}

	//Should not reach this place!
	assert(false);
	return NULL;
}

/*----------------------------------------------*/

/*----------------------------------------------*/

void Parser::displayCurrentState(std::vector<Token> stack, std::list<Token> postfix)
{
	std::size_t i = 0;

	std::cout << "Stack   | "; 
	for (i = 0; i < stack.size(); i++) {
		std::cout << stack[i].value;
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "PostFix | ";
	std::list<Token>::iterator iter;
	for ( iter = postfix.begin(); iter != postfix.end(); iter++) {
		std::cout << iter->value ;
		std::cout << " " ;
	}
	std::cout << std::endl << std::endl;;
} /* Parser::displayCurrentState */

void Parser::displayTokenVector(std::vector<Token> tokens)
{
	std::size_t i = 0;
	for (i = 0; i < tokens.size(); i++) 
	{
		std::cout << tokens[i].value << " ";
	}

	std::cout << std::endl;
} /* Parser::displayTokenVector */

bool Parser::isStackTokenHigherOrEqualPrecedence(const Token &tok, std::vector<Token> stack)
{
	int token_precedence = Token::getPrecedence(tok.type);
	int stack_precedence = Token::getPrecedence(stack.back().type);

	if (token_precedence < stack_precedence ||
	   	token_precedence == stack_precedence) 
	{
		return true;
	}
	return false;

} /* Parser::isStackTokenHigherOrEqualPrecedence */

bool Parser::isStackEqualPrecedence(Token &tok, std::vector<Token> stack)
{
	int token_precedence = Token::getPrecedence(tok.type);
	int stack_precedence = Token::getPrecedence(stack.back().type);

	if (token_precedence == stack_precedence)
       	{
		return true;
	}
	return false;
} /* Parser::isStackEqualPrecedence */

void Parser::addOperator(Token &tok, std::vector<Token> &stack, std::list<Token> &postfix)
{
	while (!stack.empty() &&
	      	tok.isLeftAssociative(tok) &&
	       	isStackTokenHigherOrEqualPrecedence(tok, stack))
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
			postfix.push_back(stack.back());
			stack.pop_back();
/*		}*/
	}
	stack.push_back(tok);
} /* Parser::addOperator */

bool Parser::frontStackIsLeftBracket(std::vector<Token> stack)
{
	return (stack.back()).isLeftBracket(stack.back());
} /* Parser::frontStackIsLeftBracket */


std::list<Token> Parser::shuntingYard(std::vector<Token> tokens)
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
	std::vector<Token> stack;
	std::list<Token>   postfix;

	if (SHOW_DETAILED_CALCULATION) {
		std::cout << "PARSING phase..." << std::endl;
		displayTokenVector(tokens);
	}

	for (std::size_t i = 0; i < tokens.size(); i++)
	{
		Token tok = tokens[i];
		if (tok.isNumber(tok) ) {
			postfix.push_back(tok);
		} else if (tok.isOperator(tok)) {
			addOperator(tok, stack, postfix);
		} else if ((stack.back()).isLeftBracket(tok) || tok.isFun(tok)) {

			stack.push_back(tok);
		} else if (tok.isRightBracket(tok)) {
			while (!stack.empty() &&
				!frontStackIsLeftBracket(stack))
			{
				postfix.push_back(stack.back());
				safePopBack<Token>(stack);
			}

			if (frontStackIsLeftBracket(stack)) {
				safePopBack<Token>(stack);

				if (!stack.empty() && tok.isFun(stack.back())) {

					postfix.push_back(stack.back());
					safePopBack<Token>(stack);
				}
			}
		}
		if (SHOW_DETAILED_CALCULATION) {
			displayCurrentState(stack, postfix);
		}
	}
	while (!stack.empty()) {
		postfix.push_back(stack.back());
		stack.pop_back();
	}
	if (SHOW_DETAILED_CALCULATION) {
		displayCurrentState(stack, postfix);
	}
	return postfix;
} /* Parser::shuntingYard */
