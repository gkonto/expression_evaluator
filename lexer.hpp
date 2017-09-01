/*
 *************************************************************************************
 *										     *
 * Author: Kontogiannis Giorgos (2017)						     *
 *										     *
 * The lexer will tokenize input against the following BNF:			     *
 *										     *
 * <expression> ::= <term> { +|- <term> }					     *
 * <term>       ::= (<symbol> | <factor>) {<operator> <symbol> | <factor>}	     *
 * <factor>     ::= <symbol> | ( '(' {-} <expression> ')' )			     *
 * <symbol>     ::= <number> | <gensymb> | <string>				     *
 * <gensymb>    ::= <alphabet> {<alphabet> | <digit>}				     *
 * <string>     ::= '"' {<alphabet> | <digit> | <operator> } '"'		     *
 * <operator>   ::= * | / | % | ^ | < | > | <= | >= | << | >> !=		     *
 * <alphabet>   ::= a | b | .. | z | A | B | .. | Z				     *
 * <digit>      ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 |  7 | 8 | 9			     *
 * <sign>       ::= + | -							     *
 * <edef>       ::= e | E							     *
 * <decimal>    ::= {<digit>} (<digit> [.] | [.] <digit>) {<digit>}		     *
 * <exponent>   ::= <edef> [<sign>] <digit> {<digit>}				     *
 * <real>       ::= [<sign>] <decimal> [<exponent>]				     *
 * <integer>    ::= [<sign>] {<digit>}						     *
 * <number>     ::= <read> | <integer>						     *
 *										     *
 * Definitions:									     *
 *										     *
 * ::= means is defined as							     *
 * | means "or"									     *
 * <> angle brackets used to surround category names				     *
 * Optional items are enclosed in meta symbol [ and ]				     *
 * Repetitive items (zero or more times) are enclosed in meta symbols { and }	     *
 * Terminals of only one character are surrounded by quotes (")			     *
 *										     *
 *************************************************************************************
*/

#ifndef LEXER_HPP
#define LEXER_HPP

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

	template<typename Iterator>
	Token &setToken(const token_type tt, const Iterator begin, 
			 const Iterator end,  const Iterator base_begin = Iterator(0));
	Token& setToken(const token_type tt, const std::string &s, const std::size_t p);
	
	template<typename Iterator>
	Token& setString(const Iterator begin, const Iterator end, const Iterator base_begin = Iterator(0));
	Token& setString(const std::string &s, const std::size_t p);
	std::string toStr(token_type t);
	bool isError() const;

	// token_type value (enum)
	token_type type;
	std::string value;
	std::size_t position;

};

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
		int insertAdditionalTokens();
	private:
		const char *base_itr_;
		const char *s_itr_;
		const char *s_end_;
		std::string expression_;
		std::vector<Token> token_list_; 
		Token eof_token_;
};


#endif


















