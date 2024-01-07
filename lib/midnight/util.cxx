#include "util.hxx"

#include <iostream>
#include <numbers>
#include <fstream>
#include <cmath>

namespace midnight
{
	inline double radians(const double angle)
	{
		return angle * (std::numbers::pi / 180);
	}

	inline double degrees(const double angle)
	{
		return angle * (std::numbers::inv_pi * 180);
	}

	const char *readFile(const char *filename)
	{
		std::ifstream fs(filename, std::ios::binary | std::ios::ate);
		fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		const long int size{static_cast<long int>(fs.tellg())};
		fs.seekg(0);
		char *text{new char[size + 2]};
		char next_char;
		for (long int c{0}; c < size; ++c) {
			fs.get(next_char);
			text[c] = next_char;
		}
		fs.close();
		text[size + 1] = '\0';
		return text;
	}

	Matrix<3, 1> cartesian3(const Polar coordinate)
	{
		Matrix<3, 1> mod{1, 1, 1};
		using std::cos, std::sin;
		const double radius{coordinate.radius};
		const double heading{coordinate.heading};
		const double pitch{coordinate.pitch};
		mod.entry(0, 0) = radius * cos(pitch) * sin(heading);
		mod.entry(1, 0) = -radius * sin(pitch);
		mod.entry(2, 0) = radius * cos(pitch) * cos(heading);
		return mod;
	}

	Matrix<4, 1> cartesian4(const Polar coordinate)
	{
		const Matrix<3, 1> mod{cartesian3(coordinate)};
		return Matrix<4, 1>{mod.entry(0, 0), mod.entry(1, 0), mod.entry(2, 0), 1};
	}

	Polar spherical(const Matrix<3, 1> coordinate)
	{
		Polar mod;
		mod.radius = std::sqrt(dot(coordinate, coordinate));
		mod.heading = std::atan2(coordinate.entry(0, 0), coordinate.entry(2, 0));
		mod.pitch = std::asin(-coordinate.entry(1, 0) / mod.radius);
		mod.canonise();
		return mod;
	}
}
