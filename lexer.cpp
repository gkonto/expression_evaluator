#include <iostream>
#include <vector>
#include <string>

#include "lexer.hpp"



bool details::is_whitespace(const char c)
{
         return (' '  == c) || ('\n' == c) ||
                ('\r' == c) || ('\t' == c) ||
                ('\b' == c) || ('\v' == c) ||
                ('\f' == c) ;
} /* details::is_whitespace */

bool details::is_operator_char(const char c)
{
	return (('+' == c) || ('-' == c) ||
		('*' == c) || ('/' == c) ||
		('^' == c) || ('(' == c) ||
	       	(')' == c));
} /* details::is_operator_char */

bool details::is_letter(const char c)
{
	return (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'));
} /* details::is_letter */

bool details::is_digit(const char c)
{
	return (('0' <= c) && (c <= '9'));
} /* details::is_digit */

bool details::is_letter_or_digit(const char c)
{
	return is_letter(c) || is_digit(c);
} /* is_letter_or_digit */

bool details::is_left_bracket(const char c)
{
	return ('(' == c);
} /* details::is_left_bracket */


bool details::is_right_bracket(const char c)
{
	return (')' == c);
} /* details::is_right_bracket */

bool details::is_bracket(const char c)
{
	return is_left_bracket(c) || is_right_bracket(c);
} /* details::is_bracket */

bool details::is_sign(const char c)
{
	return ('+' == c) || ('-' == c);
} /* details::is_sign */

bool details::imatch(const char c1, const char c2)
{
	return std::tolower(c1) == std::tolower(c2);
}

bool details::imatch(const std::string &s1, const std::string &s2)
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
}

bool details::cleanup_escapes(std::string &s)
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
}

/*********************************************************************/
/*		STRUCT TOKEN START				     */
/*********************************************************************/
template <typename Iterator>
Token &Token::set_token(const token_type tt, const Iterator begin, const Iterator end, const Iterator base_begin)
{
	type = tt;
	value.assign(begin, end);
	{
		position = std::distance(base_begin, begin);
	}
	return *this;
}

Token& Token::set_token(const token_type tt, const std::string &s, const std::size_t p)
{
	type = tt;
	value = s;
	position = p;
	return *this;
}

std::string Token::to_str(token_type t)
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
		case E_POW         : return "^";
		default            : return "UNKNOWN";

	}
}

bool Token::is_error() const
{
	return ((E_ERROR == type)      ||
		(E_ERR_SYMBOL == type) ||
		(E_ERR_NUMBER == type) ||
		(E_ERR_STRING == type)
	       );
}

void Lexer::display()
{
	std::cout << "Tokenizing expression: " << expression_ << std::endl;

	for (std::size_t i = 0; i < token_list_.size(); ++i)
	{
		Token t = token_list_[i];
		printf("Token[%02d] @ %03d  %6s  -->  '%s'\n",
			static_cast<unsigned int>(i),
			static_cast<unsigned int>(t.position),
			t.to_str(t.type).c_str(),
			t.value.c_str());
	}
	std::cout << std::endl;
}

void Lexer::skip_whitespace(void)
{
	while (!is_end(s_itr_) && details::is_whitespace(*s_itr_))
	{
		++s_itr_;
	}
} /* Lexer::skipwhitespace */

/*********************************************************************/

/*********************************************************************/
static bool comment_start(const char c0, const char c1, int &mode, int &incr)
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
} /* comment_start */

/*********************************************************************/

/*********************************************************************/
static bool comment_end(const char c0, const char c1, const int mode)
{
	return ((1 == mode) && ('\n' == c0)) ||
		((2 == mode) && ('*' == c0) && ('/' == c1));
} /* comment_end */

/*********************************************************************/

/*********************************************************************/
void Lexer::skip_comments(void)
// The following comment styles are supported:
// 1. // .... \n --> mode == 1;
// 2. #  .... \n --> mode == 2;
// 3. /* .... */ --> mode == 3;
{
	 int mode = 0;
	 int increment = 0;

	if (is_end(s_itr_) || is_end((s_itr_ + 1))) {

		 return;
	} else if (!comment_start(*s_itr_, *(s_itr_ + 1), mode, increment)) {

		 return;
	}
	
	s_itr_ += increment;

	while (!is_end(s_itr_) && !comment_end(*s_itr_, *(s_itr_ + 1), mode))
	{
		++s_itr_;
	}

	if (!is_end(s_itr_))
	{
		s_itr_ += mode;
		skip_whitespace();
		skip_comments();
	}
} /* Lexer::skip_comments */

/*********************************************************************/

/*********************************************************************/
void Lexer::scan_token()
/*
 * Detects the type of token.
 * Operator
 * Symbol
 * Number
 * String
 */
{
	skip_whitespace();
	skip_comments();

	if (is_end(s_itr_)) {
		return;
	} else if (details::is_operator_char(*s_itr_)) {
		scan_operator();
		return;
	} else if (details::is_letter(*s_itr_)) {
		scan_symbol();
		return;
	} else if (details::is_digit((*s_itr_)) || ('.' == (*s_itr_))) {
		scan_number();
		return;
	} else if ('\'' == (*s_itr_)) {
		scan_string();
		return;
	} else {
		Token t;
		t.set_token(Token::E_ERROR, s_itr_, s_itr_ + 2, base_itr_);
		token_list_.push_back(t);
		++s_itr_;
	}
} /* Lexer::scan_token */

/*********************************************************************/

/*********************************************************************/
void Lexer::scan_operator()
/* Identifies what type of operator is the token
 * Creates the token and pushes is to the token_list_
 * where all tokens are stored.
 * Then moves to the next char.
 */
{
	Token t;
	
	t.set_token(Token::token_type(*s_itr_),s_itr_,s_itr_ + 1,base_itr_);

	token_list_.push_back(t);

	++s_itr_;
} /* Lexer::scan_operator */

/*********************************************************************/

/*********************************************************************/
void Lexer::scan_symbol()
{
	const char *begin = s_itr_;
	while ((!is_end(s_itr_)) && (details::is_letter_or_digit(*s_itr_) || ((*s_itr_) == '_')))
	{
		++s_itr_;
	}
	Token t;
	t.set_token(Token::E_SYMBOL, begin, s_itr_, base_itr_);
	token_list_.push_back(t);
} /* Lexer::scan_symbol */

/*********************************************************************/

/*********************************************************************/
void Lexer::scan_number()
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

	while (!is_end(s_itr_))
	{
		if ('.' == (*s_itr_))
		{
			if (dot_found)
			{
				t.set_token(Token::E_ERROR, begin, s_itr_, base_itr_);
				token_list_.push_back(t);
				return;
			}
			dot_found = true;
			++s_itr_;
			continue;
		} else if (details::imatch('e',(*s_itr_))) {
               		const char& c = *(s_itr_ + 1);

               		if (is_end(s_itr_ + 1))
		        {
				t.set_token(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
		       } else if (('+' != c) && ('-' != c) && !details::is_digit(c)) {
                 		t.set_token(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
		       }
		       e_found = true;
		       ++s_itr_;
		       continue;
		} else if (e_found && details::is_sign(*s_itr_) && !post_e_digit_found) {
			if (post_e_sign_found) {
				t.set_token(Token::E_ERROR,begin,s_itr_,base_itr_);
				token_list_.push_back(t);
				return;
			}

			post_e_sign_found = true;
			++s_itr_;
			continue;
		} else if (e_found && details::is_digit(*s_itr_)) {
			post_e_digit_found = true;
			++s_itr_;

			continue;
		} else if (('.' != (*s_itr_)) && !details::is_digit(*s_itr_))
			break;
		else
			++s_itr_;
         }

         t.set_token( Token::E_NUMBER, begin,s_itr_,base_itr_);
         token_list_.push_back(t);
         return;

} /* Lexer::scan_number */


/*********************************************************************/

/*********************************************************************/
void Lexer::scan_string()
{
	const char *begin = s_itr_ + 1;
	Token t;
	if (std::distance(s_itr_, s_end_) < 2)
	{
		t.set_token(Token::E_ERROR, s_itr_, s_end_, base_itr_);
		token_list_.push_back(t);
		return;
	}
	++s_itr_;

	bool escaped_found = false;
	bool escaped = false;

	while (!is_end(s_itr_))
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

	if (is_end(s_itr_))
	{
		t.set_token(Token::E_ERROR, begin, s_itr_, base_itr_);
		token_list_.push_back(t);
		return;
	}

	if (!escaped_found) {
		t.set_token(Token::E_STRING, begin, s_itr_, base_itr_);
	} else {
		std::string parsed_string(begin, s_itr_);
		details::cleanup_escapes(parsed_string);
		t.set_token(Token::E_STRING, parsed_string, std::distance(base_itr_, begin));
	}
	token_list_.push_back(t);
	++s_itr_;
	return;
} /* Lexer::scan_string */

void Lexer::clear()
{
	base_itr_ = 0;
	s_itr_    = 0;
	s_end_    = 0;
	token_list_.clear();
} /* Lexer::clear */

int Lexer::insert_token_core(const Token &t0, const Token &t1, Token &new_token)
{
	bool match         = false;

	if      ((t0.type == Token::E_NUMBER  ) && (t1.type == Token::E_SYMBOL  )) {
		new_token.type     = Token::E_MUL;
		new_token.value    = "*";
		new_token.position = t1.position;
	      	match = true;
	} else if ((t0.type == Token::E_NUMBER  ) && (t1.type == Token::E_LBRACKET)) {
		new_token.type     = Token::E_MUL;
		new_token.value    = "*";
		new_token.position = t1.position;
	       	match = true;
	} else if ((t0.type == Token::E_SYMBOL  ) && (t1.type == Token::E_NUMBER  )) {
		new_token.type     = Token::E_MUL;
		new_token.value    = "*";
		new_token.position = t1.position;
		match = true;
	} else if ((t0.type == Token::E_RBRACKET) && (t1.type == Token::E_NUMBER  )) {
		new_token.type     = Token::E_MUL;
		new_token.value    = "*";
		new_token.position = t1.position;
		match = true;
	} else if ((t0.type == Token::E_RBRACKET) && (t1.type == Token::E_SYMBOL  )) {
		new_token.type     = Token::E_MUL;
		new_token.value    = "*";
		new_token.position = t1.position;
		match = true;
	// Case (-1) --> (0-1)
	} else if ((t0.type == Token::E_LBRACKET) && (t1.type == Token::E_ADD)) {
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
} /* Lexer::insert_token_core */

int Lexer::insert_additional_tokens()
{
	if (token_list_.empty()) {
		return 0;
	}
	std::size_t changes = 0;

	for (std::size_t i = 0; i < (token_list_.size() - 1); ++i)
	{
		Token t;
		int insert_index = -1;
		insert_index = insert_token_core(token_list_[i], token_list_[i+1], t);
		if ((insert_index >= 0) && (insert_index <= 2+1)) {
			token_list_.insert(token_list_.begin() + ( i + insert_index), t);
			changes++;
		}

	}

	return changes++;
} /* Lexer::insert_additional_tokens */

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

	eof_token_.set_token(Token::E_EOF, s_end_, s_end_, base_itr_);
	token_list_.clear();

/*	std::cout << "Entering analization the while loop in process" << std::endl << std::endl;*/
	while(!is_end(s_itr_))
	{
/*		std::cout << "base_itr_: " << *s_itr_  << std::endl;*/
/*		std::cout << "s_itr_ : "   <<  *s_itr_ << std::endl;*/
/*		std::cout << "s_end_ :"    << *s_end_  << std::endl;*/
		scan_token();
		if (token_list_.empty()) {
			return true;
		} else if (token_list_.back().is_error()) {
			std::cout << "ERROR" << std::endl;
			std::cout << "base_itr_: " << *s_itr_  << std::endl;
			std::cout << "s_itr_ : "   <<  *s_itr_ << std::endl;
			std::cout << "s_end_ :"    << *s_end_  << std::endl;
			return false;
		}
	}
	//insert needed tokens
	insert_additional_tokens();

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

