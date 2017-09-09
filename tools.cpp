#include <cmath>
#include <vector>
#include <iostream>

#include "tools.hpp"

bool doubleEquals(const double a, const double  b, const double epsilon)
{
	return ((std::abs(a)-std::abs(b)) < epsilon);
} /* doubleEquals */

/*----------------------------------------------*/

/*----------------------------------------------*/
