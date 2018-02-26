#include "stdafx.h"

#include <demo/RayTracerDemo.h>

#include <random>

namespace demo
{
	RayTracerDemo::RayTracerDemo()
		: plgn::Application("Ray Tracer Demo", 1920, 1080, false),
		m_width(getWidth()), m_height(getHeight())
	{
		m_renderBuffer.reserve(m_width * m_height * 3);
	}

	void RayTracerDemo::init()
	{
		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		std::string vertexSource(R"(
		#version 450

		layout(location = 0) in vec2 in_position;

		out vec2 texCoord;

		void main()
		{
			gl_Position = vec4(in_position, 0.0, 1.0);
			texCoord = in_position * 0.5 + 0.5;
		}
	)");
		std::string fragmentSource(R"(
		#version 450

		in vec2 texCoord;

		out vec4 fragColor;

		uniform sampler2D image;

		void main()
		{
			fragColor.rgb = texture(image, texCoord).rgb;
			fragColor.a = 1.0;
		}
	)");
		m_drawTextureShader = std::make_unique<plgn::Shader>(vertexSource, fragmentSource);

		std::vector<glm::vec2> positions = {
			glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f), glm::vec2(-1.0f, 1.0f),
			glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(-1.0f, 1.0f)
		};
		m_screenVao = std::make_unique<plgn::VertexArray>(6);
		m_screenVao->addVertexBuffer(new plgn::VertexBuffer(positions), 0);
	}

	void RayTracerDemo::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE))
		{
			stop();
		}

		//std::random_device rd;
		//std::default_random_engine rnd(rd());
		//std::uniform_real<float> floatDist(0.0f, 1.0f);
		//for (int y = 0; y < m_height; y++)
		//{
		//	float r = floatDist(rnd);
		//	float g = floatDist(rnd);
		//	float b = floatDist(rnd);

		//	for (int x = 0; x < m_width; x++)
		//	{
		//		m_renderBuffer[(x + y * m_width) * 3 + 0] = r;
		//		m_renderBuffer[(x + y * m_width) * 3 + 1] = g;
		//		m_renderBuffer[(x + y * m_width) * 3 + 2] = b;
		//	}
		//}

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_renderBuffer.data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RayTracerDemo::render()
	{
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		m_drawTextureShader->use();
		m_screenVao->draw();
	}

	void RayTracerDemo::dispose()
	{
	}
}
