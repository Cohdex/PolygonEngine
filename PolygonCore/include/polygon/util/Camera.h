#pragma once

#include <glm/glm.hpp>

namespace plgn
{
	class Camera
	{
	private:
		glm::vec3 m_position;
		float m_pitch;
		float m_yaw;

	public:
		Camera();
		Camera(const glm::vec3& position);
		Camera(const glm::vec3& position, float pitch, float yaw);

		const glm::vec3& getPosition() const { return m_position; }
		glm::vec3& getPosition() { return m_position; }

		const float& getPitch() const { return m_pitch; }
		float& getPitch() { return m_pitch; }

		const float& getYaw() const { return m_yaw; }
		float& getYaw() { return m_yaw; }

		void move(const glm::vec3& movement);

		glm::mat4 getViewMatrix() const;
	};
}
