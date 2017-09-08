expression_evaluator is a parser for mathematical expression speficied in infix notation.
Firstly, the algorithm seperates the given expression in tokens.
Then, it parses the tokenized expression using the stack-based algorithm shunting-yard.
Finally, it evaluates the created postfix expression.

The evaluator can calculate expressions that include '+', '-', '*', '/', exponents, as well as
basic functions (cos(), sin() and log()).

The current version is still under development and bug fixing.
