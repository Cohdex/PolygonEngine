#pragma once

#include "stdafx.h"

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;
		glm::vec3 m_viewPosition;

		std::unique_ptr<plgn::Shader> m_simpleShader;
		GLuint m_vao, m_vbo, m_ebo;
		int m_numElements;
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
