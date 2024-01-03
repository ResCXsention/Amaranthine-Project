#include "polar.hxx"

#include <cmath>
#include <numbers>
#include <iostream>

namespace midnight
{
	void Polar::canonise()
	{
		if (radius == 0) {
			pitch = 0;
			heading = 0;
			return;
		}
		if (radius < 0) {
			radius = -radius;
			heading += std::numbers::pi;
			pitch += std::numbers::pi;
		}
		constexpr double half_pi{std::numbers::pi * 0.5};
		constexpr double double_pi{std::numbers:: pi * 2};
		if (std::fabs(pitch) > half_pi) {
			pitch += half_pi;
			pitch -= double_pi * std::floor(pitch / double_pi);
			if (pitch > std::numbers::pi)
			{
				heading += std::numbers::pi;
				pitch = 3 * half_pi - pitch;
			} else {
				pitch -= half_pi;
			}
		}
		if (std::fabs(pitch) > half_pi * 0.9999) {
			heading = 0.0f;
		} else if (std::fabs(heading) > std::numbers::pi) {
			heading += (std::numbers::pi);
			heading -= double_pi * std::floor(heading / double_pi);
			heading -= std::numbers::pi;
		}
	}

	void Polar::write() const
	{
		std::cout << "( " << radius << " " << heading << " " << pitch << " )" << std::endl;
	}
}
