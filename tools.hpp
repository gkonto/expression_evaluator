#ifndef TOOLS_HPP
#define TOOLS_HPP
#pragma once

/*#define DBG*/
#include "token.hpp"

enum exprStatus
{
	eOk,
	eError,
	eNotValidExpr,
	eNoExprGiven
};

typedef enum parseErrorTypes {
	E_NONE = 0,
	E_INVALID_ARG,
	E_NO_ARG_GIVEN,
	JUST_SHOW_HELP
} parseErrorTypes;

extern bool SHOW_DETAILED_CALCULATION;
extern bool RUN_TEST_EXPRESSIONS ;
extern int FAILED_EXPRESSIONS;
extern int PASSED_EXPRESSIONS;

namespace details
{
	bool isWhitespace(const char c);
	bool isOperatorChar(const char c);
	bool isLetter(const char c);
	bool isDigit(const char c);
	bool isLetterOrDigit(const char c);
	bool isLeftBracket(const char c);
	bool isRightBracket(const char c);
	bool isBracket(const char c);
	bool isSign(const char c);
	bool iMatch(const char c1, const char c2);
	bool iMatch(const std::string &s1 , const std::string &s2);
	void cleanupEscapes(std::string &s);
}

bool doubleEquals(const double a, const double  b, const double epsilon = 0.001);

template<typename T>
extern void safePopBack(std::vector<T> &vec)
{
	#ifdef DBG
		std::cout << "vec size in safe pop back" <<  vec.size() << std::endl;
	#endif
	if (!vec.empty())
	{
		vec.pop_back();
	}
	#ifdef DBG
		std::cout << "Ended safe pop_back()" << std::endl;
	#endif
}



class Validator
{
	public:
		Validator(std::vector<Token> tokens) : token_list_(tokens)
		{
			acceptedTokenTypes.push_back(Token::E_NUMBER);
			acceptedTokenTypes.push_back(Token::E_SYMBOL);
			acceptedTokenTypes.push_back(Token::E_RBRACKET);
			acceptedTokenTypes.push_back(Token::E_LBRACKET);
			acceptedTokenTypes.push_back(Token::E_ADD);
			acceptedTokenTypes.push_back(Token::E_SUB);
			acceptedTokenTypes.push_back(Token::E_DIV);
			acceptedTokenTypes.push_back(Token::E_MUL);
			acceptedTokenTypes.push_back(Token::E_MOD);
			acceptedTokenTypes.push_back(Token::E_POW);
		}
		void setNextTokenAcceptedTypes(Token::token_type type);
		void setTokenTypesForNumber();
		void setTokenTypesForSymbol();
		void setTokenTypesForRBracket();
		void setTokenTypesForLBracket();
		void setTokenTypesForAdd();
		void clearAcceptedTokenTypes();
		void setTokenTypesForOperator();
/*		void setTokenTypesForOperatorSubOrAdd();*/
		bool isValidExpr(int &err_code);
	private:
		std::vector<Token> token_list_;
		std::vector<int> acceptedTokenTypes;
};


#endif
