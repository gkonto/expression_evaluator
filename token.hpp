#pragma once
#include <vector>
#include <string>
#include <limits>

struct Token
{
	enum token_type
	{
		E_NONE = 0,
		E_ERROR = 1,
		E_ERR_SYMBOL = 2, 
		E_ERR_NUMBER = 3,
		E_ERR_STRING = 4,
		E_ERR_SFUNC = 5,
		E_EOF = 6,
		E_NUMBER = 7,
		E_SYMBOL = 8,
		E_STRING = 9,
		E_RBRACKET = ')',
		E_LBRACKET = '(',
		E_ADD = '+',
		E_SUB = '-',
		E_DIV = '/',
		E_MUL = '*',
		E_MOD = '%',
		E_POW = '^',
	};

	Token()
	: type(E_NONE),
	value(""),
	position(std::numeric_limits<std::size_t>::max())
	{}

	Token &setToken(const token_type tt, const char *begin, 
			 const char *end,  const char *base_begin = 0);
	Token& setToken(const token_type tt, const std::string &s, const std::size_t p);
	std::string toStr(token_type t);
	bool isError() const;

	bool isLeftAssociative(const Token &tok) const;
       	bool isOperator(const Token &tok) const;
	bool isNumber(const Token &tok) const;
	bool isLeftBracket(const Token &tok) const;
	bool isFun(const Token &tok);
	bool isRightBracket(const Token &tok) const;
	int  getPrecedence(token_type tt) const;

	// token_type value (enum)
	token_type type;
	std::string value;
	std::size_t position;

};


