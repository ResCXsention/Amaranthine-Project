module;
#include <type_traits>
#include <cmath>
#include <cassert>
#include <cstddef>
#include <array>
#include <initializer_list>
#include <iostream>

export module midnight:matrix;

static const char* MIDNIGHT_ERROR_ROW_OUTRANGE = "Midnight, row out of range";
static const char* MIDNIGHT_ERROR_COLUMN_OUTRANGE = "Midnight, column out of range";
static const char* MIDNIGHT_ERROR_DIMENSION_MISMATCH = "Midnight, matrix dimensions unworkable";
static const char* MIDNIGHT_ERROR_NONSQUARE = "Midnight, this matrix isn't a square";
static const char* MIDNIGHT_ERROR_ZERO = "Midnight, determinant is zero";
static const char* MIDNIGHT_WRONG_SIZE = "Midnight, matrix construct initaliser list of wrong length";

export namespace midnight
{
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

	template<std::size_t R, std::size_t C>
	bool operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2);

	template<std::size_t R, std::size_t C>
	struct Matrix final
	{
	public:
		Matrix() = default;
		Matrix(const double fill);
		Matrix(const std::initializer_list<double> fill);
		Matrix(const Matrix<R, C> &other);
		
		Matrix<R, C> &operator=(const Matrix<R, C> &other);
		Matrix<R, C> &operator+=(const Matrix<R, C> &other);
		Matrix<R, C> &operator-=(const Matrix<R, C> &other);
		Matrix<R, C> &operator*=(const Matrix<R, C> &other);
		Matrix<R, C> &operator*=(const double other);
		Matrix<R, C> operator+(const Matrix<R, C> &other) const;
		Matrix<R, C> operator-(const Matrix<R, C> &other) const;
		Matrix<R, C> operator-() const;
		template<unsigned long OR, unsigned long OC>
		Matrix<R, OC> operator*(const Matrix<OR, OC> &other) const;
		Matrix<R, C> operator*(const double other) const;
		friend bool midnight::operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2);

		inline double &entry(const std::size_t r, const std::size_t c);
		inline const double &entry(const std::size_t r, const std::size_t c) const;
		inline auto &getData();
		inline const auto &getData() const;
		inline std::size_t rows() const;
		inline std::size_t columns() const;
		void write() const;

		Matrix<C, R> transpose() const;
		Matrix<R - 1, C - 1> minor(const std::size_t r, const std::size_t c) const;
		Matrix<R - 1, C - 1> cofactor(const std::size_t r, const std::size_t c) const;
		double determinant() const;
		Matrix<R, C> inverse() const;

	private:
		const std::size_t length{R * C};
		double data[C][R]{0};
	};
}

export namespace midnight
{
	template<std::size_t R>
	double dot(const Matrix<R, 1> v1, const Matrix<R, 1> v2)
	{
		double mod{0.0};
		for(std::size_t i{0}; i < R; ++i)
		{
			mod += v1.entry(i, 0) * v2.entry(i, 0);
		}
		return mod;
	}

	template<std::size_t R>
	double length(const Matrix<R, 1> v)
	{
		return std::sqrt(dot(v, v));
	}

	template<std::size_t R>
	Matrix<R, 1> normalise(const Matrix<R, 1> v)
	{
		const double l{length(v)};
		return v * (1 / l);
	}

	Matrix<3, 1> cross(const Matrix<3, 1> v1, const Matrix<3, 1> v2)
	{
		const double x1{v1.entry(0, 0)};
		const double y1{v1.entry(1, 0)};
		const double z1{v1.entry(2, 0)};
		const double x2{v2.entry(0, 0)};
		const double y2{v2.entry(1, 0)};
		const double z2{v2.entry(2, 0)};
		return Matrix<3, 1>{{
			y1 * z2 - y2 * z1,
			z1 * x2 - z2 * x1,
			x1 * y2 - x2 * y1
		}};
	}

	
	template<std::size_t D>
	Matrix<D, D> matrixIdentity()
	{
		Matrix<D, D> mod(0);
		for(std::size_t i{0}; i < D; ++i)
		{
			mod.entry(i, i) = 1;
		}
		return mod;
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
			(static_cast<double>(pow(xl, 2)) * (1 - cos(r))) + cos(r),
			(xl * yl) * (1 - cos(r)) + (zl * sin(r)),
			(xl * zl) * (1 - cos(r)) - (yl * sin(r)),
			0,

			(xl * yl) * (1 - cos(r)) - (zl * sin(r)),
			(static_cast<double>(pow(yl, 2)) * (1 - cos(r))) + cos(r),
			(yl * zl) * (1 - cos(r)) + (xl * sin(r)),
			0,

			(xl * zl) * (1 - cos(r)) + (yl * sin(r)),
			(yl * zl) * (1 - cos(r)) - (xl * sin(r)),
			(static_cast<double>(pow(zl, 2)) * (1 - cos(r))) + cos(r),
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
			static_cast<double>(pow(xl, 2)) * (f - 1) + 1,
			xl * yl * (f - 1),
			xl * zl * (f - 1),
			0,

			xl * yl * (f - 1),
			static_cast<double>(pow(yl, 2)) * (f - 1) + 1,
			yl * zl * (f - 1),
			0,

			xl * zl * (f - 1),
			yl * zl * (f - 1),
			static_cast<double>(pow(zl, 2)) * (f - 1) + 1,

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

export namespace midnight
{
	template<std::size_t R, std::size_t C>
	Matrix<R, C>::Matrix(const double fill)
	{
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				data[j][i] = fill;
			}
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C>::Matrix(const std::initializer_list<double> fill)
	{
		std::size_t i{0}, j{0};
		assert(fill.size() == R * C && MIDNIGHT_WRONG_SIZE);
		for(auto e{fill.begin()}; e != fill.end(); ++e)
		{
			data[j][i] = *e;
			++j;
			if(j >= C)
			{
				++i;
				j = 0;
			}
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C>::Matrix(const Matrix<R, C> &other)
	{
		auto otherData{other.getData()};
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				data[j][i] = otherData[j][i];
			}
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator=(const Matrix<R, C> &other)
	{
		auto otherData{other.getData()};
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				data[j][i] = otherData[j][i];
			}
		}
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator+=(const Matrix<R, C> &other)
	{
		*this = *this + other;
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator-=(const Matrix<R, C> &other)
	{
		*this = *this - other;
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator*=(const Matrix<R, C> &other)
	{
		*this = *this * other;
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator*=(const double other)
	{
		*this = *this * other;
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator+(const Matrix<R, C> &other) const
	{
		Matrix<R, C> mod{*this};
		auto otherData{other.getData()};
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				mod.entry(i, j) += otherData[i][j];	
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator-(const Matrix<R, C> &other) const
	{
		Matrix<R, C> mod{*this};
		auto otherData{other.getData()};
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				mod.entry(i, j) -= otherData[i][j];	
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator-() const
	{
		Matrix<R, C> mod{*this * -1.0f};
		return mod;
	}

	template<std::size_t R, std::size_t C>
	template<unsigned long OR, unsigned long OC>
	Matrix<R, OC> Matrix<R, C>::operator*(const Matrix<OR, OC> &other) const
	{
		assert(C == OR && MIDNIGHT_ERROR_DIMENSION_MISMATCH);
		Matrix<R, OC> mod(0);
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < OC; ++j)
			{
				double dotResult{0.0f};
				for(std::size_t k{0}; k < C; ++k)
				{
					dotResult += entry(i, k) * other.entry(k, j);
				}
				mod.entry(i, j) = dotResult;
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator*(const double other) const
	{
		Matrix<R, C> mod{*this};
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < C; ++j)
			{
				mod.entry(i, j) *= other;
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	bool operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2)
	{
		for(std::size_t i{0}; i < R; ++i)
		{
			if(m1.entry(i / R, i % C) != m2.entry(i / R, i % C))
			{
				return false;
			}
		}
		return true;
	}
	
	template<std::size_t R, std::size_t C>
	inline double &Matrix<R, C>::entry(const std::size_t r, const std::size_t c)
	{
		assert(r < R && MIDNIGHT_ERROR_ROW_OUTRANGE);
		assert(c < C && MIDNIGHT_ERROR_COLUMN_OUTRANGE);
		return data[c][r];
	}

	template<std::size_t R, std::size_t C>
	inline const double &Matrix<R, C>::entry(const std::size_t r, const std::size_t c) const
	{
		assert(r < R && MIDNIGHT_ERROR_ROW_OUTRANGE);
		assert(c < C && MIDNIGHT_ERROR_COLUMN_OUTRANGE);
		return data[c][r];
	}

	template<std::size_t R, std::size_t C>
	auto &Matrix<R, C>::getData()
	{
		return data;
	}

	template<std::size_t R, std::size_t C>
	inline const auto &Matrix<R, C>::getData() const
	{
		return data;
	}

	template<std::size_t R, std::size_t C>
	inline std::size_t Matrix<R, C>::rows() const
	{
		return R;
	}

	template<std::size_t R, std::size_t C>
	inline std::size_t Matrix<R, C>::columns() const
	{
		return C;
	}

	template<std::size_t R, std::size_t C>
	void Matrix<R, C>::write() const
	{
		std::cout << "[  ";
		for(int i{0}; i < R; ++i)
		{
			for(int j{0}; j < C; ++j)
			{
				std::cout << entry(i, j) << " ";
			}
			if(i < R - 1)
			{
				std::cout << "\n   ";
			}
		}
		std::cout << " ]\n" << std::flush;
	}

	template<std::size_t R, std::size_t C>
	Matrix<C, R> Matrix<R, C>::transpose() const
	{
		Matrix<C, R> mod(0);
		for(int i{0}; i < R; ++i)
		{
			for(int j{0}; j < C; ++j)
			{
				mod.entry(j, i) = entry(i, j);
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R - 1, C - 1> Matrix<R, C>::minor(const std::size_t r, const std::size_t c) const
	{
		assert(R >= 3 && MIDNIGHT_ERROR_DIMENSION_MISMATCH);
		assert(C >= 3 && MIDNIGHT_ERROR_DIMENSION_MISMATCH);
		assert(R == C && MIDNIGHT_ERROR_NONSQUARE);
		assert(r < R && MIDNIGHT_ERROR_ROW_OUTRANGE);
		assert(c < C && MIDNIGHT_ERROR_COLUMN_OUTRANGE);
		Matrix<R - 1, C - 1> mod(0);
		for(std::size_t i{0}; i < R; ++i)
		{
			if(i == r)
			{
				continue;
			}
			for(std::size_t j{0}; j < C; ++j)
			{
				if(j == c)
				{
					continue;
				}
				mod.entry(i > r ? i - 1 : i, j > c ? j - 1 : j) = entry(i, j); 
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	double Matrix<R, C>::determinant() const
	{
		assert(R == C && MIDNIGHT_ERROR_NONSQUARE);
		static auto compute = [](const Matrix<2, 2> m) -> double
		{
			const double m00{m.entry(0, 0)};
			const double m11{m.entry(1, 1)};
			const double m01{m.entry(0, 1)};
			const double m10{m.entry(1, 0)};
			return (m00 * m11) - (m01 * m10);
		};
		if constexpr(R == 2)
		{
			return compute(*this);
		}
		else
		{
			double mod{0.0f};
			for(std::size_t j{0}; j < C; ++j)
			{
				mod += this->entry(0, j) * std::pow(-1, j) * (this->minor(0, j).determinant());
			}
			return mod;
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::inverse() const
	{
		assert(this->determinant() != 0 && MIDNIGHT_ERROR_ZERO);
		Matrix<R, C> mod(0);
		for(std::size_t i{0}; i < R; ++i)
		{
			for(std::size_t j{0}; j < R; ++j)
			{
				mod.entry(i, j) = std::pow(-1, i + j) * (this->minor(i, j).determinant());
			}
		}
		return mod.transpose() * (1 / this->determinant());
	}
}
