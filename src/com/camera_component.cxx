#include "camera_component.hxx"

#include <iostream>
#include <scene.hxx>
#include <node.hxx>

namespace res
{
	void Camera::cycle(const midnight::Matrix4x4 current_transform) 
	{
		Scene *owning_scene{owning_node->get_owning_scene()};
		if (owning_scene->get_active_camera() == this) {
			owning_scene->view_matrix = (current_transform * owning_node->get_transform()).inverse();
		}
	}

	void Camera::set_active()
	{
		Scene *owning_scene{owning_node->get_owning_scene()};
		owning_scene->active_camera = this;
		update_scene_projection_matrix();
	}

	void Camera::set_fov(const float fov)
	{
		this->fov = fov;
		update_scene_projection_matrix();
	}

	float Camera::get_fov() const
	{
		return fov;
	}

	void Camera::set_aspect(const float aspect)
	{
		this->aspect = aspect;
		update_scene_projection_matrix();
	}

	float Camera::get_aspect() const
	{
		return aspect;
	}

	void Camera::set_near(const float near)
	{
		this->near = near;
		update_scene_projection_matrix();
	}

	float Camera::get_near() const
	{
		return near;
	}

	void Camera::set_far(const float far)
	{
		this->far = far;
		update_scene_projection_matrix();
	}

	float Camera::get_far() const
	{
		return far;
	}

	void Camera::update_scene_projection_matrix()
	{
		Scene *owning_scene{owning_node->get_owning_scene()};
		owning_scene->projection_matrix = midnight::matrixPerspective(fov, aspect, near, far);
	}
}
