#include <cmath>
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>

#include "tools.hpp"
#include "token.hpp"


bool doubleEquals(const double a, const double  b, const double epsilon)
{
	return ((std::abs(a - b)) < epsilon);
} /* doubleEquals */

/*----------------------------------------------*/

/*----------------------------------------------*/
bool details::isWhitespace(const char c)
{
         return (' '  == c) || ('\n' == c) ||
                ('\r' == c) || ('\t' == c) ||
                ('\b' == c) || ('\v' == c) ||
                ('\f' == c) ;
} /* details::isWhitespace */

/*----------------------------------------------*/
bool details::isOperatorChar(const char c)
{
	return (('+' == c) || ('-' == c) ||
		('*' == c) || ('/' == c) ||
		('^' == c) || ('(' == c) ||
	       	(')' == c));
} /* details::isOperatorChar */

/*----------------------------------------------*/
bool details::isLetter(const char c)
{
	return (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'));
} /* details::isLetter */

/*----------------------------------------------*/
bool details::isDigit(const char c)
{
	return (('0' <= c) && (c <= '9'));
} /* details::isDigit */

/*----------------------------------------------*/
bool details::isLetterOrDigit(const char c)
{
	return isLetter(c) || isDigit(c);
} /* details::isLetterOrDigit */

/*----------------------------------------------*/
bool details::isLeftBracket(const char c)
{
	return ('(' == c);
} /* details::isLeftBracket */

/*----------------------------------------------*/
bool details::isRightBracket(const char c)
{
	return (')' == c);
} /* details::isRightBracket */

/*----------------------------------------------*/
bool details::isBracket(const char c)
{
	return isLeftBracket(c) || isRightBracket(c);
} /* details::isBracket */

/*----------------------------------------------*/
bool details::isSign(const char c)
{
	return ('+' == c) || ('-' == c);
} /* details::isSign */

/*----------------------------------------------*/
bool details::iMatch(const char c1, const char c2)
{
	return std::tolower(c1) == std::tolower(c2);
} /* details::iMatch */

/*----------------------------------------------*/
bool details::iMatch(const std::string &s1, const std::string &s2)
{
	if (s1.size() == s2.size())
	{
		for (std::size_t i = 0; i < s1.size(); ++i)
		{
			if (std::tolower(s1[i]) != std::tolower(s2[i]))
			{
				return false;
			}
		}
		return true;
	}
	return true;
} /* details::iMatch */

/*----------------------------------------------*/
void details::cleanupEscapes(std::string &s)
{
	typedef std::string::iterator str_itr_t;

	str_itr_t itr1 = s.begin();
	str_itr_t itr2 = s.begin();
	str_itr_t end  = s.end();

	std::size_t removal_count = 0;

	while (end != itr1)
	{
		if ('\\' == (*itr1))
	       	{
			++removal_count;

			if (end == ++itr1) {
				break;
			} else if ('\\' != (*itr1)) {
				switch (*itr1)
				{
					case 'n' :
						(*itr1) = '\n';
						break;
					case 'r' :
						(*itr1) = '\r';
						break;
					case 't' :
						(*itr1) = '\t';
						break;
				}
				continue;
			}
		}

		if (itr1 != itr2)
		{
			(*itr2) = (*itr1);
		}

		++itr1;
		++itr2;
	}
	s.resize(s.size() - removal_count);
} /* details::cleanupEscapes */

/*----------------------------------------------*/

void Validator::setTokenTypesForNumber()
{
	acceptedTokenTypes.push_back(Token::E_SYMBOL);
	acceptedTokenTypes.push_back(Token::E_RBRACKET);
	acceptedTokenTypes.push_back(Token::E_ADD);
	acceptedTokenTypes.push_back(Token::E_SUB);
	acceptedTokenTypes.push_back(Token::E_DIV);
	acceptedTokenTypes.push_back(Token::E_MUL);
	acceptedTokenTypes.push_back(Token::E_MOD);
	acceptedTokenTypes.push_back(Token::E_POW);
}

void Validator::setTokenTypesForSymbol()
{
	acceptedTokenTypes.push_back(Token::E_LBRACKET);
}

void Validator::setTokenTypesForRBracket()
{
	acceptedTokenTypes.push_back(Token::E_SYMBOL);
	acceptedTokenTypes.push_back(Token::E_RBRACKET);
	acceptedTokenTypes.push_back(Token::E_ADD);
	acceptedTokenTypes.push_back(Token::E_SUB);
	acceptedTokenTypes.push_back(Token::E_DIV);
	acceptedTokenTypes.push_back(Token::E_MUL);
	acceptedTokenTypes.push_back(Token::E_MOD);
	acceptedTokenTypes.push_back(Token::E_POW);
}

void Validator::setTokenTypesForLBracket()
{
	acceptedTokenTypes.push_back(Token::E_NUMBER);
	acceptedTokenTypes.push_back(Token::E_SYMBOL);
	acceptedTokenTypes.push_back(Token::E_LBRACKET);
}

/*void Validator::setTokenTypesForOperatorSubOrAdd()*/
/*{*/
/*	acceptedTokenTypes.push_back(Token::E_NUMBER);*/
/*	acceptedTokenTypes.push_back(Token::E_SYMBOL);*/
/*	acceptedTokenTypes.push_back(Token::E_LBRACKET);*/
/*	acceptedTokenTypes.push_back(Token::E_ADD);*/
/*	acceptedTokenTypes.push_back(Token::E_SUB);*/
/*}*/

void Validator::setTokenTypesForOperator()
{
	acceptedTokenTypes.push_back(Token::E_NUMBER);
	acceptedTokenTypes.push_back(Token::E_SYMBOL);
	acceptedTokenTypes.push_back(Token::E_LBRACKET);
}

void Validator::setNextTokenAcceptedTypes(Token::token_type type)
{
	acceptedTokenTypes.clear();

	if (type == Token::E_NUMBER) {
		setTokenTypesForNumber();
	} else if (type == Token::E_SYMBOL) {
		setTokenTypesForSymbol();
	} else if (type == Token::E_RBRACKET) {
		setTokenTypesForRBracket();
	} else if (type == Token::E_LBRACKET) {
		setTokenTypesForLBracket();
/*	} else if (type == Token::E_SUB ||*/
/*		   type == Token::E_ADD)*/
/*       	{*/
/*		setTokenTypesForOperatorSubOrAdd();*/
/*		setTokenTypesForOperatorSubOrAdd();*/
	} else if (type == Token::E_DIV ||
		   type == Token::E_MUL ||
	       	   type == Token::E_MOD ||
		   type == Token::E_ADD ||
		   type == Token::E_SUB ||
	       	   type == Token::E_POW)
       	{
		setTokenTypesForOperator();
	} else {
		std::cout << std::endl;
		std::cout << "Update the validator !" << std::endl;
		assert(true);
	}
}


bool Validator::isValidExpr(int &err_code)
{
	if (token_list_.empty()) {
		err_code = eNoExprGiven;
		return false;	
	}

	for (std::size_t i = 0; i < token_list_.size(); i++)
	{
		Token tok = token_list_[i];
		std::vector<int>::iterator it;
		it = find(acceptedTokenTypes.begin(), acceptedTokenTypes.end(), tok.type);
		if ( it == acceptedTokenTypes.end())
	       	{
			err_code = eNotValidExpr;
			return false;
		} else {
			setNextTokenAcceptedTypes(tok.type);
		}
	}

	return true;
}


