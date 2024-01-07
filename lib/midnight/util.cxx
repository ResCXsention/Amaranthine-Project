#include "util.hxx"

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

	std::string readFile(const char *filename)
	{
		std::ifstream fs{filename, std::ios::binary | std::ios::ate};
		if (!fs.is_open()) {
			std::cout << "fail" << std::endl;
		}
		const auto size{fs.tellg()};
		fs.seekg(0);
		std::string text(size, '\0');
		fs.read(&text[0], size);
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
