#include <polar.hxx>

#include <cmath>
#include <numbers>
#include <iostream>

namespace midnight
{
	void Polar::canonise()
	{
		if (radius == 0) {
			depression = 0;
			heading = 0;
			return;
		}
		if (radius < 0) {
			radius = -radius;
			heading += std::numbers::pi_v<float>;
			depression += std::numbers::pi_v<float>;
		}
		constexpr float half_pi{std::numbers::pi_v<float> * 0.5};
		constexpr float double_pi{std::numbers::pi_v<float> * 2};
		if (std::fabs(depression) > half_pi) {
			depression += half_pi;
			depression -= double_pi * std::floor(depression / double_pi);
			if (depression > std::numbers::pi_v<float>)
			{
				heading += std::numbers::pi_v<float>;
				depression = 3 * half_pi - depression;
			} else {
				depression -= half_pi;
			}
		}
		if (std::fabs(depression) > half_pi * 0.9999) {
			heading = 0.0f;
		} else if (std::fabs(heading) > std::numbers::pi_v<float>) {
			heading += (std::numbers::pi_v<float>);
			heading -= double_pi * std::floor(heading / double_pi);
			heading -= std::numbers::pi_v<float>;
		}
	}

	void Polar::write() const
	{
		std::cout << "( " << radius << " " << heading << " " << depression << " )" << std::endl;
	}
}
