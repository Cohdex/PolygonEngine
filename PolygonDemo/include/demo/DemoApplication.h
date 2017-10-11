#pragma once

#include "stdafx.h"

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;

		std::unique_ptr<plgn::Shader> m_simpleShader;
		GLuint m_vbo, m_vao;
		int m_numVertices;

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
