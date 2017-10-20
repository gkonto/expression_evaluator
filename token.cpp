#include <algorithm>
#include "token.hpp"
#include "tools.hpp"
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
		case E_NUMBER      : return "NUMBER";
		case E_SYMBOL      : return "SYMBOL";
		case E_STRING      : return "STRING";
		case E_RBRACKET    : return ")";
		case E_LBRACKET    : return "(";
		case E_LCRLBRACKET : return "{";
		case E_LSQRBRACKET : return "[";
		case E_ADD         : return "+";
		case E_SUB         : return "-";
		case E_DIV         : return "/";
		case E_MUL         : return "*";
		case E_MOD         : return "%";
		case E_POW         : return "**";
		case E_EQ          : return "=";
		default            : return "UNKNOWN";

	}
} /* Token::toStr */

bool Token::isFun(const Token &tok)
{
	std::vector<std::string>::iterator it;
	it = find(builtinFuns.begin(), builtinFuns.end(), tok.value);

	if (tok.type == Token::E_SYMBOL && it != builtinFuns.end()) {
		return true;
	} else {
		return false;
	}
}

bool Token::isError() const
{
	return ((E_ERROR == type)      ||
		(E_ERR_SYMBOL == type) ||
		(E_ERR_NUMBER == type) ||
		(E_ERR_STRING == type)
	       );
} /* Token::isError */

bool Token::isLeftAssociative(const Token &tok) 
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD );
} /* Token::isLeftAssociative */

bool Token::isOperator(const Token &tok)
{
	return (tok.type == Token::E_SUB ||
		tok.type == Token::E_ADD ||
		tok.type == Token::E_MUL ||
		tok.type == Token::E_DIV ||
		tok.type == Token::E_MOD ||
		tok.type == Token::E_POW );
} /* Token::isOperator */

bool Token::isNumber(const Token &tok)
{
	return (tok.type == Token::E_NUMBER);
} /* Token::isNumber */

void Token::clear()
{
	type     = E_NONE;
	value    = "";
	position = std::numeric_limits<std::size_t>::max();
}

bool Token::isSub(const Token &tok)
{
	return (tok.type == Token::E_SUB);
} /* Token::isNodeSub */

bool Token::isAdd(const Token &tok)
{
	return (tok.type == Token::E_ADD);
} /* Token::isNodeAdd */

bool Token::isMul(const Token &tok)
{
	return (tok.type == Token::E_MUL);
} /* Token::isNodeMul */

bool Token::isDiv(const Token &tok)
{
	return (tok.type == Token::E_DIV);
} /* Token::isNodeDiv */

bool Token::isMod(const Token &tok)
{
	return (tok.type == Token::E_MOD);
} /* Token::isNodeMod */

bool Token::isPow(const Token &tok)
{
	return (tok.type == Token::E_POW);
} /* Token::isNodePow */

bool Token::isAssign(const Token &tok) 
{
	return (tok.type == Token::E_EQ);
} /* Token::isAssign */

bool Token::isLeftBracket(const Token &tok)
{
	return (tok.type == Token::E_LBRACKET);
} /* Token::isLeftBracket */


bool Token::isRightBracket(const Token &tok)
{
	return (tok.type == Token::E_RBRACKET);
} /* Token::isRightBracket */

bool Token::isBracket(const Token &tok)
{
	return (isRightBracket(tok) || isLeftBracket(tok));
} /* Token::isBracket */

int Token::getPrecedence(Token::token_type tt)
{
	//TODO save in the Token Struct the precedence !
	if ( tt == Token::E_ADD || tt == Token::E_SUB) {
		return 2;
	} else if ( tt == Token::E_MUL || tt == Token::E_DIV || tt == Token::E_MOD) {
		return 3;
	} else if (tt == Token::E_POW) {
		return 4;	
	} else {
		return 0;
	}
} /* Token::getPrecedence */

