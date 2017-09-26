#ifndef PARSER_HPP
#define PARSER_HPP
#pragma once

#include <iostream>
#include <vector>
#include <list>
#include "token.hpp"

class Parser
{
	public:
		Parser(std::vector<Token> tokens): token_list_(tokens) {}
		void shuntingYard();
		std::list<Token> getPostFix() { return postfix_; }
	private:
		void displayCurrentState();
		void displayTokenVector();
		void addOperator(Token &tok);
		bool frontStackIsLeftBracket();
		bool isStackTokenHigherOrEqualPrecedence(const Token &tok);
		bool isStackEqualPrecedence(Token &tok);
	
		std::vector<Token> token_list_;
		std::vector<Token> stack_;
		std::list<Token> postfix_;
};


#endif
