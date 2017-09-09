expression_evaluator is a parser for mathematical expression speficied in infix notation.
Firstly, the algorithm seperates the given expression in tokens.
Then, it parses the tokenized expression using the stack-based algorithm shunting-yard.
Finally, it evaluates the created postfix expression.

The evaluator can calculate expressions that include '+', '-', '*', '/', exponents, as well as
basic functions (cos(), sin() and log()).

The current version is still under testing, development and bug fixing.


How to build the evaluator:
	1) Copy the files in a directory.
	2) Create a build/ folder in this directory
	3) cd build/
	4) call -> cmake ../
	5) make (while in build/)

How to run the evaluator:
	1) After build expression_evaluator is created
	2) just type ./expression_evaluator

The evaluator can take 3 arguments:
	--help                 :shows the available arguments with description

	-f [filename]          : This argument followed by a file(full files path) 
			         can calculate all the expressions of the file.
			         The file (.txt) should have the following format:
			         (line1): 'expression'		'value'
			         (line2): 'expression2'		'value2'

	-d [no, yes]           : Shows the detailed calculation from the beginning to the end. (default : yes) [NOT WORKING YET]
	-t [run, do_not_run]   : Runs the built in expressions (just for testing). (default : run)


IMPORTANT NOTE: The evaluator is still under testing.
That is, when using the -f argument tha user must give both the expression and the expected value.
Soon, the expected value will not be needed since it is what we ask for the evaluator to do.
