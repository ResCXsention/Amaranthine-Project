#include "matrix.hxx"

namespace midnight
{
	Matrix<3, 1> cross(const Matrix<3, 1> v1, const Matrix<3, 1> v2)
	{
		const double x1{v1.entry(0, 0)};
		const double y1{v1.entry(1, 0)};
		const double z1{v1.entry(2, 0)};
		const double x2{v2.entry(0, 0)};
		const double y2{v2.entry(1, 0)};
		const double z2{v2.entry(2, 0)};
		return Matrix<3, 1>{
			y1 * z2 - y2 * z1,
			z1 * x2 - z2 * x1,
			x1 * y2 - x2 * y1
		};
	}

	Matrix<4, 4> matrixRotation(const Matrix<3, 1> line, const double angle)
	{
		using std::sin, std::cos, std::pow;
		const double r{angle};
		const Matrix<3, 1> l(normalise(line));
		const double xl{l.entry(0, 0)};
		const double yl{l.entry(1, 0)};
		const double zl{l.entry(2, 0)};
		return Matrix<4, 4>{
			(pow(xl, 2) * (1 - cos(r))) + cos(r),
			(xl * yl) * (1 - cos(r)) + (zl * sin(r)),
			(xl * zl) * (1 - cos(r)) - (yl * sin(r)),
			0,

			(xl * yl) * (1 - cos(r)) - (zl * sin(r)),
			(pow(yl, 2) * (1 - cos(r))) + cos(r),
			(yl * zl) * (1 - cos(r)) + (xl * sin(r)),
			0,

			(xl * zl) * (1 - cos(r)) + (yl * sin(r)),
			(yl * zl) * (1 - cos(r)) - (xl * sin(r)),
			(pow(zl, 2) * (1 - cos(r))) + cos(r),
			0,

			0, 0, 0, 1
		};
	}

	Matrix<4, 4> matrixScale(const Matrix<3, 1> line, const double factor)
	{
		using std::pow;
		const Matrix<3, 1> l(normalise(line));
		const double xl{l.entry(0, 0)};
		const double yl{l.entry(1, 0)};
		const double zl{l.entry(2, 0)};
		const double f{factor};
		return Matrix<4, 4>{
			pow(xl, 2) * (f - 1) + 1,
			xl * yl * (f - 1),
			xl * zl * (f - 1),
			0,

			xl * yl * (f - 1),
			pow(yl, 2) * (f - 1) + 1,
			yl * zl * (f - 1),
			0,

			xl * zl * (f - 1),
			yl * zl * (f - 1),
			pow(zl, 2) * (f - 1) + 1,

			0, 0, 0, 1
		};
	}

	Matrix<4, 4> matrixTranslation(const Matrix<3, 1> line)
	{
		return Matrix<4, 4>{
			1, 0, 0, line.entry(0, 0),
			0, 1, 0, line.entry(1, 0),
			0, 0, 1, line.entry(2, 0),
			0, 0, 0, 1
		};
	}
}
