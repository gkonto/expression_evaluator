#ifndef TOOLS_HPP
#define TOOLS_HPP
#pragma once


/*#define DBG*/

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


#endif
