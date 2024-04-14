#ifndef RES_RESOURCE_TEXT
#define RES_RESOURCE_TEXT

#include <resource.hxx>

namespace res
{
	class TextResource final : public Resource
	{
	public:
		virtual void load(const std::string path) override;
		virtual void unload() override;

		const char *get_text() const;

	private:
		char *text;
	};
}

#endif
