module;

#include <numbers>
#include <cmath>

export module midnight:utils;
import :matrix;
import :polar;
import :quaternion;

export namespace midnight
{
	double radians(const double degrees);
	double degrees(const double radians);

	Matrix<3, 1> cartesian3(const Polar coordinate);
	Matrix<4, 1> cartesian4(const Polar coordinate);
	Polar spherical(const Matrix<3, 1> coordinate);
	double lerp(const double n1, const double n2, const double t);
	Quaternion slerp(const Quaternion q1, const Quaternion q2, const double t);
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

	double lerp(const double n1, const double n2, const double t)
	{
		return n1 * (1 - t) + n2* t;
	}

	Quaternion slerp(const Quaternion q1, const Quaternion q2, const double t)
	{
		Quaternion w1{q1}, w2{q2};
		double cosOmega{dot(w1, w2)};
		if(cosOmega < 0l)
		{
			w1 = -w1;
			cosOmega = -cosOmega;
		}

		double k1, k2;
		if(cosOmega > 0.9999)
		{
			k1 = 1.0 - t;
			k2 = t;
		}
		else
		{
			double sinOmega{std::sqrt(1.0 - cosOmega * cosOmega)};
			double omega{std::atan2(sinOmega, cosOmega)};
			double invSinOmega{1 / sinOmega};
			k1 = std::sin((1 - t) * omega) * invSinOmega;
			k2 = std::sin(t * omega) * invSinOmega;
		}
		Quaternion mod{
			w1.getW() * k1 + w2.getW() * k2,
			w1.getX() * k1 + w2.getX() * k2,
			w1.getY() * k1 + w2.getY() * k2,
			w1.getZ() * k1 + w2.getZ() * k2
		};
		return mod;
	}
}
