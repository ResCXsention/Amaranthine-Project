#include <matrix.hxx>

namespace midnight
{
	template<std::size_t R>
	float dot(const Matrix<R, 1> v1, const Matrix<R, 1> v2)
	{
		float mod{0};
		for (std::size_t i{0}; i < R; ++i) {
			mod += v1.entry(i, 0) * v2.entry(i, 0);
		}
		return mod;
	}

	template<std::size_t R>
	float length(const Matrix<R, 1> v)
	{
		return std::sqrt(dot(v, v));
	}

	template<std::size_t R>
	Matrix<R, 1> normalise(const Matrix<R, 1> v)
	{
		const float l{length(v)};
		return v * (1 / l);
	}
	
	template<std::size_t D>
	Matrix<D, D> matrixIdentity()
	{
		Matrix<D, D> mod;
		for (std::size_t i{0}; i < D; ++i) {
			mod.entry(i, i) = 1;
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C>::Matrix(const std::initializer_list<float> fill)
	{
		std::size_t i{0}, j{0};
		assert(fill.size() == R * C && MIDNIGHT_WRONG_SIZE);
		for (auto e{fill.begin()}; e != fill.end(); ++e) {
			entry(i, j) = *e;
			++j;
			if (j >= C) {
				++i;
				j = 0;
			}
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C>::Matrix(const Matrix<R, C> &other)
	{
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < C; ++j) {
				entry(i, j) = other.entry(i, j);
			}
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> &Matrix<R, C>::operator=(const Matrix<R, C> &other)
	{
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < C; ++j) {
				entry(i, j) = other.entry(i, j);
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
	Matrix<R, C> &Matrix<R, C>::operator*=(const float other)
	{
		*this = *this * other;
		return *this;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator+(const Matrix<R, C> &other) const
	{
		Matrix<R, C> mod{*this};
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < C; ++j) {
				mod.entry(i, j) += other.entry(i, j);
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator-() const
	{
		return *this * -1;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator-(const Matrix<R, C> &other) const
	{
		return *this + -other;
	}

	template<std::size_t R, std::size_t C>
	template<unsigned long OR, unsigned long OC>
	Matrix<R, OC> Matrix<R, C>::operator*(const Matrix<OR, OC> &other) const
	{
		assert(C == OR && MIDNIGHT_ERROR_DIMENSION_MISMATCH);
		Matrix<R, OC> mod;
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < OC; ++j) {
				float dot_result{0};
				for (std::size_t k{0}; k < C; ++k) {
					dot_result += entry(i, k) * other.entry(k, j);
				}
				mod.entry(i, j) = dot_result;
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::operator*(const float other) const
	{
		Matrix<R, C> mod{*this};
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < C; ++j) {
				mod.entry(i, j) *= other;
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	bool operator==(const Matrix<R, C> &m1, const Matrix<R, C> &m2)
	{
		for (std::size_t i{0}; i < R; ++i) {
			if (m1.entry(i / R, i % C) != m2.entry(i / R, i % C)) {
				return false;
			}
		}
		return true;
	}
	
	template<std::size_t R, std::size_t C>
	inline float &Matrix<R, C>::entry(const std::size_t r, const std::size_t c)
	{
		assert(r < R && MIDNIGHT_ERROR_ROW_OUTRANGE);
		assert(c < C && MIDNIGHT_ERROR_COLUMN_OUTRANGE);
		return data[C * c + r];
	}

	template<std::size_t R, std::size_t C>
	inline const float &Matrix<R, C>::entry(const std::size_t r, const std::size_t c) const
	{
		assert(r < R && MIDNIGHT_ERROR_ROW_OUTRANGE);
		assert(c < C && MIDNIGHT_ERROR_COLUMN_OUTRANGE);
		return data[C * c + r];
	}

	template<std::size_t R, std::size_t C>
	inline const float* const Matrix<R, C>::dataPtr() const
	{
		return reinterpret_cast<const float*>(&data[0]);
	}

	template<std::size_t R, std::size_t C>
	void Matrix<R, C>::write() const
	{
		std::cout << "[  ";
		for (int i{0}; i < R; ++i) {
			for (int j{0}; j < C; ++j) {
				std::cout << entry(i, j) << " ";
			}
			if (i < R - 1) {
				std::cout << "\n   ";
			}
		}
		std::cout << " ]\n" << std::flush;
	}

	template<std::size_t R, std::size_t C>
	Matrix<C, R> Matrix<R, C>::transpose() const
	{
		Matrix<C, R> mod;
		for (int i{0}; i < R; ++i) {
			for (int j{0}; j < C; ++j) {
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
		Matrix<R - 1, C - 1> mod;
		for (std::size_t i{0}; i < R; ++i) {
			if (i == r) {
				continue;
			}
			for (std::size_t j{0}; j < C; ++j)
			{
				if (j == c) {
					continue;
				}
				mod.entry(i > r ? i - 1 : i, j > c ? j - 1 : j) = entry(i, j); 
			}
		}
		return mod;
	}

	template<std::size_t R, std::size_t C>
	float Matrix<R, C>::determinant() const
	{
		assert(R == C && MIDNIGHT_ERROR_NONSQUARE);
		static auto compute{[](const Matrix<2, 2> m) -> float {
			const float m00{m.entry(0, 0)};
			const float m11{m.entry(1, 1)};
			const float m01{m.entry(0, 1)};
			const float m10{m.entry(1, 0)};
			return (m00 * m11) - (m01 * m10);
		}};
		if constexpr (R == 2) {
			return compute(*this);
		}	else {
			float mod{0};
			for (std::size_t j{0}; j < C; ++j) {
				mod += this->entry(0, j) * std::pow(-1, j) * (this->minor(0, j).determinant());
			}
			return mod;
		}
	}

	template<std::size_t R, std::size_t C>
	Matrix<R, C> Matrix<R, C>::inverse() const
	{
		assert(this->determinant() != 0 && MIDNIGHT_ERROR_ZERO);
		Matrix<R, C> mod;
		for (std::size_t i{0}; i < R; ++i) {
			for (std::size_t j{0}; j < R; ++j) {
				mod.entry(i, j) = std::pow(-1, i + j) * (minor(i, j).determinant());
			}
		}
		return mod.transpose() * (1 / this->determinant());
	}
}
