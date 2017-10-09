#pragma once

#include <polygon/app/Application.h>

namespace demo
{
	class DemoApplication : public plgn::Application
	{
	private:
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
