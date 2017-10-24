#include "stdafx.h"

#include <glm/gtx/transform.hpp>

namespace plgn
{
	Camera::Camera()
		: Camera(glm::vec3(0.0f), 0.0f, 0.0f)
	{
	}

	Camera::Camera(const glm::vec3& position)
		: Camera(position, 0.0f, 0.0f)
	{
	}

	Camera::Camera(const glm::vec3& position, float pitch, float yaw)
		: m_position(position), m_pitch(pitch), m_yaw(yaw)
	{
	}

	void Camera::move(const glm::vec3& movement)
	{
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 right(glm::cos(m_yaw), 0.0f, glm::sin(m_yaw));
		glm::vec3 forward = glm::cross(up, right);

		m_position += right * movement.x;
		m_position += up * movement.y;
		m_position += forward * movement.z;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		glm::mat4 viewMatrix = glm::rotate(-m_yaw, glm::vec3(0, 1, 0));
		viewMatrix = glm::rotate(viewMatrix, m_pitch, glm::vec3(1, 0, 0));
		viewMatrix = glm::inverse(viewMatrix);
		return glm::translate(viewMatrix, -m_position);
	}
}
