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
		void shunting_yard();
		std::list<Token> GetPostfix() { return postfix_; }
	private:
		void display_current_state();
		void display_token_vector();
		void add_operator(const Token &tok);
		int get_precedence(Token::token_type tt);
		bool is_left_associative(const Token &tok);
		bool is_number(const Token &tok);
		bool is_left_bracket(const Token &tok);
		bool is_operator(const Token &tok);
		bool front_stack_is_left_bracket();
		bool is_right_bracket(const Token &tok);
		bool is_stack_token_higher_or_equal_precedence(const Token &tok);
	
		std::vector<Token> token_list_;
		std::vector<Token> stack_;
		std::list<Token> postfix_;
};


#endif
