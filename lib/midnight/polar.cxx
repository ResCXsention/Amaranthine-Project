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
			heading += std::numbers::pi_v<float>;
			pitch += std::numbers::pi_v<float>;
		}
		constexpr float half_pi{std::numbers::pi_v<float> * 0.5};
		constexpr float double_pi{std::numbers::pi_v<float> * 2};
		if (std::fabs(pitch) > half_pi) {
			pitch += half_pi;
			pitch -= double_pi * std::floor(pitch / double_pi);
			if (pitch > std::numbers::pi_v<float>)
			{
				heading += std::numbers::pi_v<float>;
				pitch = 3 * half_pi - pitch;
			} else {
				pitch -= half_pi;
			}
		}
		if (std::fabs(pitch) > half_pi * 0.9999) {
			heading = 0.0f;
		} else if (std::fabs(heading) > std::numbers::pi_v<float>) {
			heading += (std::numbers::pi_v<float>);
			heading -= double_pi * std::floor(heading / double_pi);
			heading -= std::numbers::pi_v<float>;
		}
	}

	void Polar::write() const
	{
		std::cout << "( " << radius << " " << heading << " " << pitch << " )" << std::endl;
	}
}
