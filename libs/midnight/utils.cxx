module;

#include <numbers>
#include <cmath>

export module midnight:utils;
import :matrix;
import :polar;

export namespace midnight
{
	double radians(const double degrees);
	double degrees(const double radians);

	Matrix<3, 1> cartesian3(const Polar coordinate);
	Matrix<4, 1> cartesian4(const Polar coordinate);
	Polar spherical(const Matrix<3, 1> coordinate);
}

export namespace midnight
{
	double radians(const double degrees)
	{
		return degrees * (std::numbers::pi / 180.0f);
	}

	double degrees(const double radians)
	{
		return radians * (std::numbers::inv_pi * 180.0f);
	}

	Matrix<3, 1> cartesian3(const Polar coordinate)
	{
		Matrix<3, 1> mod(1);
		using std::cos, std::sin;
		const double radius{coordinate.getRadius()};
		const double heading{coordinate.getHeading()};
		const double pitch{coordinate.getPitch()};
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
		mod.setRadius(std::sqrt(dot(coordinate, coordinate)));
		mod.setHeading(std::atan2(coordinate.entry(0, 0), coordinate.entry(2, 0)));
		mod.setPitch(std::asin(-coordinate.entry(1, 0) / mod.getRadius()));
		mod.canonise();
		return mod;
	}
}
