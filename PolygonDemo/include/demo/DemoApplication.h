#pragma once

#include "stdafx.h"

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;
		plgn::Camera m_camera;

		std::unique_ptr<plgn::VertexArray> m_torusMesh;
		std::unique_ptr<plgn::VertexArray> m_teapotMesh;
		std::unique_ptr<plgn::VertexArray> m_airplaneMesh;
		std::unique_ptr<plgn::VertexArray> m_spiderMesh;
		std::unique_ptr<plgn::Shader> m_simpleShader;
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
