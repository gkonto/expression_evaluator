#include "token.hpp"
/*********************************************************************/
/*		STRUCT TOKEN START				     */
/*********************************************************************/
Token &Token::setToken(const token_type tt, const char *begin, const char *end, const char *base_begin)
{
	type = tt;
	value.assign(begin, end);
	{
		position = std::distance(base_begin, begin);
	}
	return *this;
} /* &Token::setToken */

Token& Token::setToken(const token_type tt, const std::string &s, const std::size_t p)
{
	type = tt;
	value = s;
	position = p;
	return *this;
} /* Token::setToken */

std::string Token::toStr(token_type t)
{
	switch (t)
	{
		case E_NONE        : return "NONE";
		case E_ERROR       : return "ERROR";
		case E_ERR_SYMBOL  : return "ERROR_SYMBOL";
		case E_ERR_NUMBER  : return "ERROR_NUMBER";
		case E_ERR_STRING  : return "ERROR_STRING";
		case E_EOF         : return "EOF";
		case E_NUMBER      : return "NUMBER";
		case E_SYMBOL      : return "SYMBOL";
		case E_STRING      : return "STRING";
		case E_RBRACKET    : return ")";
		case E_LBRACKET    : return "(";
		case E_ADD         : return "+";
		case E_SUB         : return "-";
		case E_DIV         : return "/";
		case E_MUL         : return "*";
		case E_MOD         : return "%";
		case E_POW         : return "**";
		default            : return "UNKNOWN";

	}
} /* Token::toStr */

bool Token::isError() const
{
	return ((E_ERROR == type)      ||
		(E_ERR_SYMBOL == type) ||
		(E_ERR_NUMBER == type) ||
		(E_ERR_STRING == type)
	       );
} /* Token::isError */


