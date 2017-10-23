#ifndef PARSER_HPP
#define PARSER_HPP
#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include "token.hpp"

class Node
{
	public:
		virtual void build(std::vector<Token>&tokens) {};
		virtual void build(std::vector<Node *> &stack) {};
		virtual double eval() { return 0; }
		virtual int   getPrecedence() { return 0; }
		virtual void  setToken(const Token &tok) {};
		virtual Token getToken();
		virtual void setLhs(Node *node) {};
		virtual void setRhs(Node *node) {};
		virtual Node *getLhs() { return NULL; }
		virtual Node *getRhs() { return NULL; }
	private:
		std::vector<Node *> program_;
};

class BinaryNode : public Node
{
	public:
		void setToken(const Token &tok) { token_ = tok; }
		Token getToken() { return token_; }
		void setLhs(Node *node)   { lhs_ = node; }
		void setRhs(Node *node)   { rhs_ = node; }
		Node *getRhs() { return rhs_; }
		Node *getLhs() { return lhs_; }
		void createBinaryNode(Token tok)
		{
			token_ = tok;
			lhs_   = NULL;
			rhs_   = NULL;
		};
	private:
		Token token_;
		Node *lhs_;
		Node *rhs_;
};

class ExpressionNode : public Node
{
	public:
		ExpressionNode() : e_node(NULL) {}
		void build(std::vector<Token>&tokens);
		double eval();
	private:
		void addOperatorToStack(Node *node, std::vector<Node *>&stack, std::vector<Node *>&postfix);
		bool isStackNodeHigherOrEqualPrecedenceFromNode(Node *node, std::vector<Node *>&stack);

		Node *e_node;
};

class Number: public Node
{
	public:
		Number(Token tok) : token_(tok) {}
		void build(std::vector<Node *>&nodes) {}
		void setToken(const Token &tok) { token_ = tok; }
		double eval();
		Token getToken() { return token_; }
	private:
		Token token_;
};

class Bracket: public Node
{
	public:
		Bracket(Token tok) : token_(tok) {}
		Token getToken() { return token_; }
		void  setToken(Token tok) { token_ = tok; }
	private:
		Token token_;
};

class SubOp : public BinaryNode
{
	public:
		SubOp(Token tok)
		{
			createBinaryNode(tok);
		};
		void build(std::vector<Node *>&nodes);
		double eval();
		int   getPrecedence() { return 2; }
	private:

/*		Token token_;*/
/*		Node *lhs_;*/
/*		Node *rhs_;*/
};

class AddOp : public BinaryNode
{
	public:
		AddOp(Token tok)
		{
			createBinaryNode(tok);
		};
		void build(std::vector<Node *>&nodes);
		double eval();
		int   getPrecedence() { return 2; }
	private:
/*		Token token_;*/
/*		Node *lhs_;*/
/*		Node *rhs_;*/
};

class MulOp : public BinaryNode
{
	public:
		MulOp(Token tok)
		{
			createBinaryNode(tok);
		};
		void build(std::vector<Node *>&nodes);
		double eval();
		int   getPrecedence() { return 3; }
	private:
/*		Token token_;*/
/*		Node *lhs_;*/
/*		Node *rhs_;*/
};

class DivOp : public BinaryNode
{
	public:
		DivOp(Token tok)
		{
			createBinaryNode(tok);
		};
		void build(std::vector<Node *>&nodes);
		double eval();
		int   getPrecedence() { return 3; }
	private:
/*		Token token_;*/
/*		Node *lhs_;*/
/*		Node *rhs_;*/
};

/*class ModOp : public BinaryNode*/
/*{*/
/*	public:*/
/*		ModOp(Token tok) :token_(tok), lhs_(NULL), rhs_(NULL) {}*/
/*		void build(std::vector<Node *>&nodes);*/
/*//		double eval();*/
/*		int   getPrecedence() { return 3; }*/
/*	private:*/
/*};*/

class PowOp : public BinaryNode
{
	public:
		PowOp(Token tok)
		{
			createBinaryNode(tok);
		};
		void build(std::vector<Node *>&nodes);
		double eval();
		int   getPrecedence() { return 4; }
	private:
/*		Token token_;*/
/*		Node *lhs_;*/
/*		Node *rhs_;*/
};

class BracketChecker
{
	public:
		BracketChecker() :  state_(true) {}
		std::size_t checkBracketValidity(std::vector<Token> &tokens);
		bool result() { return state_ && stack_.empty(); }
		Token errorToken() { return errorToken_; }
		void reset();
		bool operator()(const Token &t);
	private:
		bool state_;
		std::stack<char> stack_;
		Token errorToken_;
};


class Parser
{
	public:
		Parser() {}

		Node *createNode(std::vector<Token> &tokens);
		static Node *createNode(const Token &tok);
		BracketChecker getBracketChecker() { return bracketChecker_; }
		static void displayTokenVector(std::vector<Token> tokens);
		static void displayCurrentState(std::vector<Token> stack, std::list<Token> postfix);
		static void addOperator(Token &tok, std::vector<Token> &stack, std::list<Token> &postfix);
		static void addOperator(Token &tok, std::vector<Node *> &stack, std::vector<Node *> &postfix);
		static bool frontStackIsLeftBracket(std::vector<Token>);
		static bool frontStackIsLeftBracket(std::vector<Node *>);
	private:
		static bool isStackTokenHigherOrEqualPrecedence(const Token &tok, std::vector<Token> stack);
		bool isStackEqualPrecedence(Token &tok, std::vector<Token> stack);
		BracketChecker bracketChecker_;

};

#endif

