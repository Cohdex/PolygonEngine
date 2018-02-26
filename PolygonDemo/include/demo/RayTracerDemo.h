#pragma once

#include "stdafx.h"

#include <vector>

namespace demo
{
	class RayTracerDemo : public plgn::Application
	{
	private:
		std::vector<float> m_renderBuffer;
		int m_width;
		int m_height;

		GLuint m_textureId;
		std::unique_ptr<plgn::Shader> m_drawTextureShader;
		std::unique_ptr<plgn::VertexArray> m_screenVao;

	public:
		RayTracerDemo();
		RayTracerDemo(const RayTracerDemo&) = delete;
		RayTracerDemo& operator=(const RayTracerDemo&) = delete;

	private:
		void init() override;
		void update(double deltaTime) override;
		void render() override;
		void dispose() override;
	};
}
