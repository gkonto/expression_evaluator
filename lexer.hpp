#ifndef LEXER_HPP
#define LEXER_HPP
#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <limits>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>


#include "token.hpp"

class Lexer
{
	public:
		Lexer():base_itr_(0), s_itr_(0), s_end_(0), expression_("")
		{
			clear();
		}
		void clear();
		bool process(const std::string &str);
		bool empty() const;
		std::size_t size() const;
		void begin();
		Token &operator[](const std::size_t &index);
		Token operator[](const std::size_t &index) const;
		std::vector<Token> getTokens() { return token_list_; }
		void display();
	private:
		bool isEnd(const char *itr) { return (s_end_ == itr); }
		void skipWhitespace();
		void skipComments();
		void scanToken();
		void scanOperator();
		void scanSymbol();
		void scanNumber();
		void scanString();

		int insertTokenCore(const Token &t0, const Token &t1, Token &new_token);
		int substituteTokensCore(const Token &t0, const Token &t1, Token &new_token);
		int insertAdditionalTokens();
		int substituteTokens();
	private:
		const char *base_itr_;
		const char *s_itr_;
		const char *s_end_;
		std::string expression_;
		std::vector<Token> token_list_; 
		Token eof_token_;
};


#endif


















