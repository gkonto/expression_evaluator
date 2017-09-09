#include <cmath>
#include <vector>
#include <iostream>

#include "tools.hpp"

template<typename T>
void safePopBack(std::vector<T> vec)
{
	if (!vec.empty()) {
		vec.pop_back();
	}
} /* safePopBack */

bool doubleEquals(const double a, const double  b, const double epsilon)
{
	return ((std::abs(a)-std::abs(b)) < epsilon);
} /* doubleEquals */

/*----------------------------------------------*/

/*----------------------------------------------*/
