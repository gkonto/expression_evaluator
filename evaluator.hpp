#ifndef EVALUATE_HPP
#define EVALUATE_HPP
#pragma once

#include <iostream>
#include <vector>
#include "lexer.hpp"

class Evaluator
{
	public:
		Evaluator(std::list<Token> postfix): postfix_(postfix) {}
		void displayStack();
		std::vector<Token> evaluate();
	private:
		std::list<Token> postfix_;
		std::vector<Token> stack_;
};

#endif
