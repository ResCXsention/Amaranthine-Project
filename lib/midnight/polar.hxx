#ifndef LIB_MIDNIGHT_POLAR
#define LIB_MIDNIGHT_POLAR

namespace midnight
{
	struct Polar final
	{
	public:
		double radius{0};
		double heading{0};
		double pitch{0};

		void canonise();
		void write() const;

	};
}

#endif
