#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <list>
#include "lexer.hpp"

class Parser
{
	public:
		Parser(std::vector<Token> tokens): token_list_(tokens) {}
		void shuntingYard();
		std::list<Token> getPostFix() { return postfix_; }
	private:
		void displayCurrentState();
		void displayTokenVector();
		void addOperator(const Token &tok);
		int getPrecedence(Token::token_type tt);
		bool isLeftAssociative(const Token &tok);
		bool isNumber(const Token &tok);
		bool isLeftBracket(const Token &tok);
		bool isOperator(const Token &tok);
		bool frontStackIsLeftBracket();
		bool isRightBracket(const Token &tok);
		bool isStackTokenHigherOrEqualPrecedence(const Token &tok);
		bool isFun(const Token &tok);
	
		std::vector<Token> token_list_;
		std::vector<Token> stack_;
		std::list<Token> postfix_;
};


#endif
