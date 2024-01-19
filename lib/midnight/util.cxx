#include "util.hxx"

#include <iostream>
#include <numbers>
#include <fstream>
#include <cmath>

namespace midnight
{
	inline float radians(const float angle)
	{
		return angle * (std::numbers::pi_v<float> / 180);
	}

	inline float degrees(const float angle)
	{
		return angle * (std::numbers::inv_pi * 180);
	}

	const char *readFile(const char *filename)
	{
		std::ifstream fs(filename, std::ios::ate);
		fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		const long size{fs.tellg()};
		fs.seekg(0);
		char *text{new char[size + 1]};
		char next_char;
		for (long int c{0}; c < size; ++c) {
			fs.get(next_char);
			text[c] = next_char;
		}
		fs.close();
		text[size] = '\0';
		std::cout << text << std::endl;
		return text;
	}

	Matrix<3, 1> cartesian3(const Polar coordinate)
	{
		Matrix<3, 1> mod{1, 1, 1};
		using std::cos, std::sin;
		const float radius{coordinate.radius};
		const float heading{coordinate.heading};
		const float depression{coordinate.depression};
		mod.entry(0, 0) = radius * cos(depression) * sin(heading);
		mod.entry(1, 0) = -radius * sin(depression);
		mod.entry(2, 0) = radius * cos(depression) * cos(heading);
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
		mod.depression = std::asin(-coordinate.entry(1, 0) / mod.radius);
		mod.canonise();
		return mod;
	}
}
