#ifndef LIB_MIDNIGHT_UTIL
#define LIB_MIDNIGHT_UTIL

#include "matrix.hxx"
#include "polar.hxx"

namespace midnight
{
	inline double radians(const double angle);
	inline double degrees(const double angle);
	const char *readFile(const char *filename);

	Matrix<3, 1> cartesian3(const Polar coordinate);
	Matrix<4, 1> cartesian4(const Polar coordinate);
	Polar spherical(const Matrix<3, 1> coordinate);
}

#endif
