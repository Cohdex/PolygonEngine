#pragma once

#include "stdafx.h"

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;
		glm::vec3 m_viewPosition;
		float m_pitch, m_yaw;
		float m_distance = 1.5f;

		std::unique_ptr<plgn::Shader> m_simpleShader;
		std::unique_ptr<plgn::VertexArray> m_torusMesh;
		std::unique_ptr<plgn::VertexArray> m_modelMesh;
		std::unique_ptr<plgn::Texture2D> m_texture;

	public:
		DemoApplication();
		DemoApplication(const DemoApplication&) = delete;
		DemoApplication& operator=(const DemoApplication&) = delete;

	private:
		void init() override;
		void update(double deltaTime) override;
		void render() override;
		void dispose() override;
	};
}
