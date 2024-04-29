#ifndef RES_CAMERA_COMPONENT
#define RES_CAMERA_COMPONENT

#include <component.hxx>
#include <midnight.hxx>

namespace res
{
	class Camera : public Component
	{
	public:
		virtual void cycle(const midnight::Matrix4x4 current_transform) override;
		
		void set_active();
		void set_fov(const float fov);
		float get_fov() const;
		void set_aspect(const float aspect);
		float get_aspect() const;
		void set_near(const float near);
		float get_near() const;
		void set_far(const float far);
		float get_far() const;

	private:
		float fov{0.57};
		float aspect{800 / 600};
		float near{0.001}, far{2000.0};

		void update_scene_projection_matrix();
	};
}

#endif
