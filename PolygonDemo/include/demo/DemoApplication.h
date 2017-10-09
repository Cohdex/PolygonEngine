#pragma once

#include "stdafx.h"

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
		glm::mat4 m_projectionMatrix;

		GLuint m_simpleShader;
		GLuint m_vbo, m_vao;
		int m_numVertices;

	public:
		DemoApplication();

	private:
		void init() override;
		void update(double deltaTime) override;
		void render() override;
	};
}
