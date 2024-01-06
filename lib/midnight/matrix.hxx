#ifndef LIB_MIDNIGHT_MATRIX
#define LIB_MIDNIGHT_MATRIX

#include <cmath>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>

namespace midnight
{
	static const char *MIDNIGHT_ERROR_ROW_OUTRANGE{"Midnight, row out of range"};
	static const char *MIDNIGHT_ERROR_COLUMN_OUTRANGE{"Midnight, column out of range"};
	static const char *MIDNIGHT_ERROR_DIMENSION_MISMATCH{"Midnight, matrix dimensions unworkable"};
	static const char *MIDNIGHT_ERROR_NONSQUARE{"Midnight, this matrix isn't a square"};
	static const char *MIDNIGHT_ERROR_ZERO{"Midnight, determinant is zero"};
	static const char *MIDNIGHT_WRONG_SIZE{"Midnight, matrix construct initaliser list of wrong length"};

	template<std::size_t R, std::size_t C>
	struct Matrix;

	typedef Matrix<2, 1> Vector2;
	typedef Matrix<3, 1> Vector3;
	typedef Matrix<4, 1> Vector4;
	typedef Matrix<2, 2> Matrix2x2;
	typedef Matrix<3, 3> Matrix3x3;
	typedef Matrix<4, 4> Matrix4x4;

	template<std::size_t R>
	double dot(const Matrix<R, 1> v1, const Matrix<R, 1> v2);
	template<std::size_t R>
	double length(const Matrix<R, 1> v);
	template<std::size_t R>
	Matrix<R, 1> normalise(const Matrix<R, 1> v);
	Matrix<3, 1> cross(const Matrix<3, 1> v1, const Matrix<3, 1> v2);

	template<std::size_t D>
	Matrix<D, D> matrixIdentity();
	Matrix<4, 4> matrixRotation(const Matrix<3, 1> line, const double angle);
	Matrix<4, 4> matrixScale(const Matrix<3, 1> line, const double factor);
	Matrix<4, 4> matrixTranslation(const Matrix<3, 1> line);
	Matrix<4, 4> matrixPerspective(const double fov, const double aspect, const double near, const double far);
	Matrix<4, 4> matrixOrthographic(const double width, const double height, const double near, const double far);

	template<std::size_t R, std::size_t C>
	bool operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2);

	template<std::size_t R, std::size_t C>
	struct Matrix final {
	public:
		Matrix() = default;
		Matrix(const std::initializer_list<double> fill);
		Matrix(const Matrix<R, C> &other);
		
		Matrix<R, C> &operator=(const Matrix<R, C> &other);
		Matrix<R, C> &operator+=(const Matrix<R, C> &other);
		Matrix<R, C> &operator-=(const Matrix<R, C> &other);
		Matrix<R, C> &operator*=(const Matrix<R, C> &other);
		Matrix<R, C> &operator*=(const double other);
		Matrix<R, C> operator+(const Matrix<R, C> &other) const;
		Matrix<R, C> operator-() const;
		Matrix<R, C> operator-(const Matrix<R, C> &other) const;
		template<std::size_t OR, std::size_t OC>
		Matrix<R, OC> operator*(const Matrix<OR, OC> &other) const;
		Matrix<R, C> operator*(const double other) const;

		inline double &entry(const std::size_t r, const std::size_t c);
		inline const double &entry(const std::size_t r, const std::size_t c) const;
		inline auto &dataPtr();
		inline const auto &dataPtr() const;
		void write() const;

		Matrix<C, R> transpose() const;
		Matrix<R - 1, C - 1> minor(const std::size_t r, const std::size_t c) const;
		double determinant() const;
		Matrix<R, C> inverse() const;

	private:
		const std::size_t length{R * C};
		double data[R][C]{0};

		friend bool operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2);
	};
}

#include "matrix.txx"

#endif
