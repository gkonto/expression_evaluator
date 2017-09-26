#include <iostream>
#include <vector>
#include <string>

#include "lexer.hpp"
#include "tools.hpp"
#include "token.hpp"


void Lexer::display()
{
	std::cout << "Tokenizing expression: " << expression_ << std::endl;

	for (std::size_t i = 0; i < token_list_.size(); ++i)
	{
		Token t = token_list_[i];
		printf("Token[%02d] @ %03d  %6s  -->  '%s'\n",
			static_cast<unsigned int>(i),
			static_cast<unsigned int>(t.position),
			t.toStr(t.type).c_str(),
			t.value.c_str());
	}
	std::cout << std::endl;
} /* Lexer::display */

void Lexer::skipWhitespace(void)
{
	while (!isEnd(s_itr_) && details::isWhitespace(*s_itr_))
	{
		++s_itr_;
	}
} /* Lexer::skipWhitespace */

/*********************************************************************/

/*********************************************************************/
static bool commentStart(const char c0, const char c1, int &mode, int &incr)
{
	mode = 0;
	if ( '#' == c0) {
		mode = 1;
		incr = 1;
	} else if ('/' == c0) {
		if ('/' == c1) {
			mode = 1;
			incr = 2;
		} else if ( '*' == c1) {
			mode = 2;
			incr = 2;
		}
	}
	return (mode != 0);
} /* commentStart */

/*********************************************************************/

/*********************************************************************/
static bool commentEnd(const char c0, const char c1, const int mode)
{
	return ((1 == mode) && ('\n' == c0)) ||
		((2 == mode) && ('*' == c0) && ('/' == c1));
} /* commentEnd */

/*********************************************************************/

/*********************************************************************/
void Lexer::skipComments(void)
// The following comment styles are supported:
// 1. // .... \n --> mode == 1;
// 2. #  .... \n --> mode == 2;
// 3. /* .... */ --> mode == 3;
{
	 int mode = 0;
	 int increment = 0;

	if (isEnd(s_itr_) || isEnd((s_itr_ + 1))) {

		 return;
	} else if (!commentStart(*s_itr_, *(s_itr_ + 1), mode, increment)) {

		 return;
	}
	
	s_itr_ += increment;

	while (!isEnd(s_itr_) && !commentEnd(*s_itr_, *(s_itr_ + 1), mode))
	{
		++s_itr_;
	}

	if (!isEnd(s_itr_))
	{
		s_itr_ += mode;
		skipWhitespace();
		skipComments();
	}
} /* Lexer::skipComments */

/*********************************************************************/

/*********************************************************************/
void Lexer::scanToken()
/*
 * Detects the type of token.
 * Operator
 * Symbol
 * Number
 * String
 */
{
	skipWhitespace();
	skipComments();

	if (isEnd(s_itr_)) {
		return;
	} else if (details::isOperatorChar(*s_itr_)) {
		scanOperator();
		return;
	} else if (details::isLetter(*s_itr_)) {
		scanSymbol();
		return;
	} else if (details::isDigit((*s_itr_)) || ('.' == (*s_itr_))) {
		scanNumber();
		return;
	} else if ('\'' == (*s_itr_)) {
		scanString();
		return;
	} else {
		Token t;
		t.setToken(Token::E_ERROR, s_itr_, s_itr_ + 2, base_itr_);
		token_list_.push_back(t);
		++s_itr_;
	}
} /* Lexer::scanToken */

/*********************************************************************/

/*********************************************************************/
void Lexer::scanOperator()
/* Identifies what type of operator is the token
 * Creates the token and pushes is to the token_list_
 * where all tokens are stored.
 * Then moves to the next char.
 */
{
	Token t;
	
	if (!isEnd(s_itr_ + 1))
	{
		Token::token_type ttype = Token::E_NONE;

		char c0 = s_itr_[0];
		char c1 = s_itr_[1];

		if ((c0 == '*') && (c1 == '*')) ttype = Token::E_POW;
		
		if (Token::E_NONE != ttype)
		{
			t.setToken(ttype, s_itr_, s_itr_ + 2, base_itr_);
			token_list_.push_back(t);
			s_itr_ += 2;
			return;
		}
	}

	t.setToken(Token::token_type(*s_itr_),s_itr_,s_itr_ + 1,base_itr_);

	token_list_.push_back(t);

	++s_itr_;
} /* Lexer::scanOperator */

/*********************************************************************/

/*********************************************************************/
void Lexer::scanSymbol()
{
	const char *begin = s_itr_;
	while ((!isEnd(s_itr_)) && (details::isLetterOrDigit(*s_itr_) || ((*s_itr_) == '_')))
	{
		++s_itr_;
	}
	Token t;
	std::string tmp_str(begin, s_itr_);
	t.setToken(Token::E_SYMBOL, begin, s_itr_, base_itr_);
	token_list_.push_back(t);
} /* Lexer::scanSymbol */

/*********************************************************************/

/*********************************************************************/
void Lexer::scanNumber()
{
/*
 * Attempt to match a valid numeric value in one of the following formats:
 * 1. 123456
 * 2. 123.456
 * 3. 123.456e3
 * 4. 123.456E3
 * 5. 123.456e+3
 * 6. 123.456E+3
 * 7. 123.456e-3
 * 8. 123.456E-3
 */
	const char * begin      = s_itr_;
	bool dot_found          = false;
	bool e_found            = false;
	bool post_e_sign_found  = false;
	bool post_e_digit_found = false;
	Token t;

	while (!isEnd(s_itr_))
	{
		if ('.' == (*s_itr_))
		{
			if (dot_found)
			{
				t.setToken(Token::E_ERROR, begin, s_itr_, base_itr_);
				token_list_.push_back(t);
				return;
			}
			dot_found = true;
			++s_itr_;
			continue;
		} else if (details::iMatch('e',(*s_itr_))) {
               		const char& c = *(s_itr_ + 1);

               		if (isEnd(s_itr_ + 1))
		        {
				t.setToken(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
		       } else if (('+' != c) && ('-' != c) && !details::isDigit(c)) {
                 		t.setToken(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
		       }
		       e_found = true;
		       ++s_itr_;
		       continue;
		} else if (e_found && details::isSign(*s_itr_) && !post_e_digit_found) {
			if (post_e_sign_found) {
				t.setToken(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
			}

			post_e_sign_found = true;
			++s_itr_;
			continue;
		} else if (e_found && details::isDigit(*s_itr_)) {
			post_e_digit_found = true;
			++s_itr_;

			continue;
		} else if (('.' != (*s_itr_)) && !details::isDigit(*s_itr_))
			break;
		else
			++s_itr_;
         }

         t.setToken( Token::E_NUMBER, begin,s_itr_,base_itr_);
         token_list_.push_back(t);
         return;

} /* Lexer::scanNumber */


/*********************************************************************/

/*********************************************************************/
void Lexer::scanString()
{
	const char *begin = s_itr_ + 1;
	Token t;
	if (std::distance(s_itr_, s_end_) < 2)
	{
		t.setToken(Token::E_ERROR, s_itr_, s_end_, base_itr_);
		token_list_.push_back(t);
		return;
	}
	++s_itr_;

	bool escaped_found = false;
	bool escaped = false;

	while (!isEnd(s_itr_))
	{
		if (!escaped && ('\\' == *s_itr_))
		{
			escaped_found = true;
			escaped = true;
			++s_itr_;
			continue;
		} else if (!escaped) {
			if ('\'' == *s_itr_)
				break;
		} else if (escaped) {
			escaped = false;
		}
		++s_itr_;
	}

	if (isEnd(s_itr_))
	{
		t.setToken(Token::E_ERROR, begin, s_itr_, base_itr_);
		token_list_.push_back(t);
		return;
	}

	if (!escaped_found) {
		t.setToken(Token::E_STRING, begin, s_itr_, base_itr_);
	} else {
		std::string parsed_string(begin, s_itr_);
		details::cleanupEscapes(parsed_string);
		t.setToken(Token::E_STRING, parsed_string, std::distance(base_itr_, begin));
	}
	token_list_.push_back(t);
	++s_itr_;
	return;
} /* Lexer::scanString */

void Lexer::clear()
{
	base_itr_ = 0;
	s_itr_    = 0;
	s_end_    = 0;
	token_list_.clear();
} /* Lexer::clear */

int Lexer::insertTokenCore(const Token &t0, const Token &t1, Token &new_token)
{
	bool match         = false;

	// 2log(2) --> 2*log(2)
/*	if      ((t0.type == Token::E_NUMBER  ) && (t1.type == Token::E_SYMBOL  )) {*/
/*		new_token.type     = Token::E_MUL;*/
/*		new_token.value    = "*";*/
/*		new_token.position = t1.position;*/
/*	      	match = true;*/
	// 2(2) --> 2*(2)	
/*	} else if ((t0.type == Token::E_NUMBER  ) && (t1.type == Token::E_LBRACKET)) {*/
/*		new_token.type     = Token::E_MUL;*/
/*		new_token.value    = "*";*/
/*		new_token.position = t1.position;*/
/*	       	match = true;*/
	// x2 --> x*2
/*	} else if ((t0.type == Token::E_SYMBOL  ) && (t1.type == Token::E_NUMBER  )) {*/
/*		new_token.type     = Token::E_MUL;*/
/*		new_token.value    = "*";*/
/*		new_token.position = t1.position;*/
/*		match = true;*/
/*	} else if ((t0.type == Token::E_RBRACKET) && (t1.type == Token::E_NUMBER  )) {*/
/*		new_token.type     = Token::E_MUL;*/
/*		new_token.value    = "*";*/
/*		new_token.position = t1.position;*/
/*		match = true;*/
/*	} else if ((t0.type == Token::E_RBRACKET) && (t1.type == Token::E_SYMBOL  )) {*/
/*		new_token.type     = Token::E_MUL;*/
/*		new_token.value    = "*";*/
/*		new_token.position = t1.position;*/
/*		match = true;*/
	// Case (-1) --> (0-1)
/*	} else if ((t0.type == Token::E_LBRACKET) && (t1.type == Token::E_ADD)) {*/
	if ((t0.type == Token::E_LBRACKET) && (t1.type == Token::E_ADD)) {
		new_token.type     = Token::E_NUMBER;
		new_token.value    = "0";
		new_token.position = t1.position;
		match = true;
	} else if ((t0.type == Token::E_LBRACKET) && (t1.type == Token::E_SUB)) {
		new_token.type     = Token::E_NUMBER;
		new_token.value    = "0";
		new_token.position = t1.position;
		match = true;
	}

	return (match) ? 1 : -1;
} /* Lexer::insertTokenCore */

int Lexer::insertAdditionalTokens()
{
	if (token_list_.empty()) {
		return 0;
	}
	std::size_t changes = 0;

	for (std::size_t i = 0; i < (token_list_.size() - 1); ++i)
	{
		Token t;
		int insert_index = -1;
		insert_index = insertTokenCore(token_list_[i], token_list_[i+1], t);
		if ((insert_index >= 0) && (insert_index <= 2+1)) {
			token_list_.insert(token_list_.begin() + ( i + insert_index), t);
			changes++;
		}

	}

	return changes++;
} /* Lexer::insertAdditionalTokens */


int Lexer::substituteTokensCore(const Token &t0, const Token &t1, Token &new_token)
{
	bool match = false;

	if ((t0.type == Token::E_ADD && t1.type == Token::E_SUB) ||
	    (t0.type == Token::E_SUB && t1.type == Token::E_ADD)) {
		new_token.type     = Token::E_SUB;
		new_token.value    = "-";
		new_token.position = t0.position;
		match = true;
	} else if ((t0.type == Token::E_ADD && t1.type == Token::E_ADD) ||
		   (t0.type == Token::E_SUB && t1.type == Token::E_SUB)) {
		new_token.type     = Token::E_ADD;
		new_token.value    = "+";
		new_token.position = t0.position;
		match = true;
	}

	return (match) ? 1 : -1;
} /* Lexer::substituteTokensCore */


int Lexer::substituteTokens()
{
	if (token_list_.empty()) {
		return 0;
	}
	std::size_t changes = 0;

	std::size_t i = 0;

	while ( i < (token_list_.size() - 1))
       	{
		Token t;
		
		int substitute_index = -1;
		substitute_index = substituteTokensCore(token_list_[i], token_list_[i+1], t);
		if (substitute_index > 0) {
			std::vector<Token>::iterator it;

			token_list_.erase(token_list_.begin() + i, token_list_.begin() + i + 2);	
			it = token_list_.begin() + i;
			token_list_.insert(token_list_.begin() + i, static_cast<const Token &>(t));
			changes++;

		}
		if (substitute_index == -1) {
			i++;
		}
	}	
	return changes;
}


bool Lexer::process(const std::string &str)
/* Beginning of everything.
 * A string is given in this member fun.
 * And private member are initialized.
 * Loop each char and identifies what type of token it is,
 * then moves to the next one.
 */
{
	expression_ = str;
	base_itr_ = str.data();
	s_itr_	  = str.data();
	// FIXME Accessing the value at data()+size() produces undefined behavior:
	// There are no guarantees that a null character terminates the character
	// sequence pointed by the value returned by this function.
	// See string::c_str for a function that provides such guarant
	s_end_    = str.data() + str.size();

/*	std::cout << "base_itr_: " << s_itr_  << std::endl;*/
/*	std::cout << "s_itr_ : "   <<  s_itr_ << std::endl;*/
/*	std::cout << "s_end_ :"    << s_end_  << std::endl;*/

	eof_token_.setToken(Token::E_EOF, s_end_, s_end_, base_itr_);
	token_list_.clear();

/*	std::cout << "Entering analization the while loop in process" << std::endl << std::endl;*/
	while(!isEnd(s_itr_))
	{
/*		std::cout << "base_itr_: " << *s_itr_  << std::endl;*/
/*		std::cout << "s_itr_ : "   <<  *s_itr_ << std::endl;*/
/*		std::cout << "s_end_ :"    << *s_end_  << std::endl;*/
		scanToken();
		if (token_list_.empty()) {
			return true;
		} else if (token_list_.back().isError()) {
			std::cout << "ERROR" << std::endl;
			std::cout << "base_itr_: " << *s_itr_  << std::endl;
			std::cout << "s_itr_ : "   <<  *s_itr_ << std::endl;
			std::cout << "s_end_ :"    << *s_end_  << std::endl;
			return false;
		}
	}

	//substitute
	substituteTokens();	

	//insert needed tokens
	insertAdditionalTokens();

	return true;
} /* Lexer::process */

bool Lexer::empty() const
{
	return token_list_.empty();
} /* Lexer::empty */

std::size_t Lexer::size() const
{
	return token_list_.size();
} /* Lexer::size */

void Lexer::begin()
{
} /* Lexer::begin */

Token &Lexer::operator[](const std::size_t &index)
{
	if (index < token_list_.size()) {
		return token_list_[index];
	} else {
		return eof_token_;
	}
} /* &Lexer::operator[] */

Token Lexer::operator[](const std::size_t &index) const
{
	if (index < token_list_.size()) {
		return token_list_[index];
	} else {
		return eof_token_;
	}
} /* Lexer::operator[] */

