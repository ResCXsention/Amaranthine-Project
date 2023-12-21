module;

#include <cmath>
#include <numbers>
#include <iostream>

export module midnight:polar;

export namespace midnight
{
	struct Polar final
	{
	public :
		Polar() = default;
		Polar(const double radius, const double heading, const double pitch);

		void setRadius(const double radius);
		void setHeading(const double heading);
		void setPitch(const double pitch);
		inline double getRadius() const;
		inline double getHeading() const;
		inline double getPitch() const;
		void canonise();
		void write() const;
	
	private :
		double radius{0.0};
		double heading{0.0};
		double pitch{0.0};
	};
}

namespace midnight
{
	Polar::Polar(const double radius, const double heading, const double pitch)
	: radius{radius}, heading{heading}, pitch{pitch}
	{
	}

	void Polar::setRadius(const double radius)
	{
		this->radius = radius;
	}

	void Polar::setHeading(const double heading)
	{
		this->heading = heading;
	}

	void Polar::setPitch(const double pitch)
	{
		this->pitch = pitch;
	}

	inline double Polar::getRadius() const
	{
		return radius;
	}

	inline double Polar::getHeading() const
	{
		return heading;
	}

	inline double Polar::getPitch() const
	{
		return pitch;
	}

	void Polar::write() const
	{
		std::cout << "( " << radius << " " << heading << " " << pitch << " )" << std::endl;
	}

	void Polar::canonise()
	{
		if(radius == 0.0f)
		{
			pitch = 0.0f;
			heading = 0.0f;
			return;
		}
		if(radius < 0.0f)
		{
			radius = -radius;
			heading += std::numbers::pi;
			pitch += std::numbers::pi;
		}
		constexpr double halfPi{std::numbers::pi * 0.5};
		constexpr double doublePi{std::numbers:: pi * 2};
		if(std::fabs(pitch) > halfPi)
		{
			pitch += halfPi;
			pitch -= doublePi * std::floor(pitch / doublePi);
			if(pitch > std::numbers::pi)
			{
				heading += std::numbers::pi;
				pitch = 3 * halfPi - pitch;
			}
			else
			{
				pitch -= halfPi;
			}
		}
		if(std::fabs(pitch) > halfPi * 0.9999)
		{
			heading = 0.0f;
		}
		else if(std::fabs(heading) > std::numbers::pi)
		{
			heading += (std::numbers::pi);
			heading -= doublePi * std::floor(heading / doublePi);
			heading -= std::numbers::pi;
		}
	}
}


