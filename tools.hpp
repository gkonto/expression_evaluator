#ifndef TOOLS_HPP
#define TOOLS_HPP
#pragma once


typedef enum parseErrorTypes {
	E_NONE = 0,
	E_INVALID_ARG,
	E_NO_ARG_GIVEN,
	JUST_SHOW_HELP
} parseErrorTypes;


extern bool SHOW_DETAILED_CALCULATION;
extern bool RUN_TEST_EXPRESSIONS ;

bool doubleEquals(const double a, const double  b, const double epsilon = 0.001);

template<typename T>
void safePopBack(std::vector<T> vec);


#endif
