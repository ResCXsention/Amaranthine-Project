module;

#include <cmath>
#include <numbers>
#include <iostream>

export module midnight:quaternion;
import :matrix;

export namespace midnight
{
	struct Quaternion;

	double dot(const Quaternion q1, const Quaternion q2);
	Quaternion quaternionIdentity();
	Quaternion quaternionExponentiate(const Quaternion q, const double p);
	Quaternion quaternionAngleBetween(const Matrix<3, 1> v1, const Matrix<3, 1> v2);
	Quaternion quaternionRotation(const double angle, const Matrix<3, 1> axis);
	Matrix<3, 1> quaternionRotateVector(const Matrix<3, 1> v, const Quaternion q);

	struct Quaternion final
	{
	public:
		Quaternion() = default;
		Quaternion(const double angle, const double x, const double y, const double z);
		Quaternion(const double angle, const Matrix<3, 1> n);

		Quaternion operator*=(const Quaternion other);
		Quaternion operator-() const;
		Quaternion operator*(const Quaternion other) const;
		Quaternion operator*(const double other) const;

		double getW() const;
		double getX() const;
		double getY() const;
		double getZ() const;
		double length() const;
		Quaternion conjugate() const;
		Quaternion inverse() const;
		void write() const;

	private:
		double w{0.0};
		double x{0.0};
		double y{0.0};
		double z{0.0};

		friend double dot(const Quaternion q1, const Quaternion q2);
		friend Quaternion quaternionExponentiate(const Quaternion q, const double exponent);
		friend Quaternion quaternionAngleBetween(const Matrix<3, 1> v1, const Matrix<3, 1> v2);
		friend Quaternion quaternionRotation(const double angle, const Matrix<3, 1> axis);
		friend Matrix<3, 1> quaternionRotateVector(const Matrix<3, 1> v, const Quaternion q);
	};
}

export namespace midnight
{
	double dot(const Quaternion q1, const Quaternion q2)
	{
		return q1.w * q2.w + dot(Matrix<3, 1>{q1.x, q2.y, q1.z}, Matrix<3, 1>{q2.x, q2.y, q2.z});
	}

	Quaternion quaternionIdentity()
	{
		return Quaternion{1, 0, 0, 0};
	}

	Quaternion quaternionExponentiate(const Quaternion q, const double exponent)
	{
		Quaternion mod{q};
		if(std::fabs(q.w) < 0.9999)
		{
			const double halfAngle{std::acos(mod.w)};
			const double newHalfAngle{halfAngle * exponent};
			const double t{std::sin(newHalfAngle) / std::sin(halfAngle)};
			mod.w = newHalfAngle;
			mod.x *= t;
			mod.y *= t;
			mod.z *= t;
		}
		return mod;
	}

	Quaternion quaternionAngleBetween(const Matrix<3, 1> v1, const Matrix<3, 1> v2)
	{
		Quaternion mod{1, 0, 0, 0};
		const Matrix<3, 1> n1(normalise(v1));
		const Matrix<3, 1> n2(normalise(v2));
		if(dot(n1, n2) == 1)
		{
			return mod;
		}
		else if(dot(n1, n2) == -1)
		{
			mod = quaternionRotation(std::numbers::pi, cross(n1, Matrix<3, 1>{1, 0, 0})) * mod;
			return mod;
		}
		mod.w = 1 + (dot(n1, n2));
		const Matrix<3, 1> c(cross(n1, n2));
		mod.x = c.entry(0, 0);
		mod.y = c.entry(1, 0);
		mod.z = c.entry(2, 0);
		return mod * (1 / mod.length());
	}

	Quaternion quaternionRotation(const double angle, const Matrix<3, 1> axis)
	{
		Quaternion mod;
		const Matrix<3, 1> n(normalise(axis));
		using std::cos, std::sin;
		mod.w = cos(angle / 2);
		mod.x = sin(angle / 2) * n.entry(0, 0);
		mod.y = sin(angle / 2) * n.entry(1, 0);
		mod.z = sin(angle / 2) * n.entry(2, 0);
		return mod;
	}

	Matrix<3, 1> quaternionRotateVector(const Matrix<3, 1> v, const Quaternion q)
	{
		const Quaternion mod{q * Quaternion{0, v.entry(0, 0), v.entry(1, 0), v.entry(2, 0)} * q.inverse()};
		mod.write();
		return Matrix<3, 1>{mod.x, mod.y, mod.z};
	}
}

export namespace midnight
{
	Quaternion::Quaternion(const double angle, const double x, const double y, const double z)
	: w{angle}, x{x}, y{y}, z{z}
	{
	}

	Quaternion::Quaternion(const double angle, const Matrix<3, 1> n)
	: Quaternion(angle, n.entry(0, 0), n.entry(1, 0), n.entry(2, 0))
	{
	}

	Quaternion Quaternion::operator*=(const Quaternion other)
	{
		*this = *this * other;
		return *this;
	}

	Quaternion Quaternion::operator-() const
	{
		return Quaternion{-w, -x, -y, -z};
	}

	Quaternion Quaternion::operator*(const Quaternion other) const
	{
		Quaternion mod;
		mod.w = w * other.w - x * other.x - y * other.y - z * other.z;

		mod.x = w * other.x + x * other.w + y * other.z - z * other.y;
		mod.y = w * other.y + y * other.w + z * other.x - x * other.z;
		mod.z = w * other.z + z * other.w + x * other.y - y * other.x;
		return mod;
	}

	Quaternion Quaternion::operator*(const double other) const
	{
		return Quaternion{w * other, x * other, y * other, z * other};
	}

	double Quaternion::getW() const
	{
		return w;
	}

	double Quaternion::getX() const
	{
		return x;
	}

	double Quaternion::getY() const
	{
		return y;
	}

	double Quaternion::getZ() const
	{
		return z;
	}

	double Quaternion::length() const
	{
		return std::sqrt(w * w + x * x + y * y + z * z);
	}

	Quaternion Quaternion::conjugate() const
	{
		return Quaternion{w, -x, -y, -z};
	}

	Quaternion Quaternion::inverse() const
	{
		const double l{length()};
		if(l == 1)
		{
			return Quaternion{w, -x, -y, -z};
		}
		else
		{
			return Quaternion{w, -x, -y, -z} * (1 / (l * l));
		}
	}

	void Quaternion::write() const
	{
		std::cout << "[ " << w << " ( " << x << " " << y << " " << z <<  " ) ]" << std::endl;
	}
}
