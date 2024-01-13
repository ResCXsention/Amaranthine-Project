#ifndef LIB_MIDNIGHT_POLAR
#define LIB_MIDNIGHT_POLAR

namespace midnight
{
	struct Polar final
	{
	public:
		float radius{0};
		float heading{0};
		float depression{0};

		void canonise();
		void write() const;

	};
}

#endif
