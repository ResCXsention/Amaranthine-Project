#ifndef RES_DRAWABLE_COMPONENT
#define RES_DRAWABLE_COMPONENT

#include <memory>
#include <component.hxx>
#include <model_resource.hxx>

namespace res
{
	class Drawable : public Component
	{
	public:
		virtual void cycle() override;
		
		void set_shader(const unsigned int shader);
		void set_model(std::weak_ptr<res::ModelResource> model);
	
	protected:
		std::weak_ptr<res::ModelResource> model;
		unsigned int shader{0};
	};
}

#endif
