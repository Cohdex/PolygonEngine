#include "stdafx.h"

#include <demo/DemoApplication.h>

static const std::string simpleVertexShader = R"(
	#version 450 core
	#line 18

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec2 in_texCoord;
	layout(location = 2) in vec4 in_color;

	out VS_OUT
	{
		vec2 texCoord;
		vec4 color;
	} vs_out;

	uniform mat4 projectionMatrix = mat4(1.0);
	uniform mat4 viewMatrix = mat4(1.0);

	void main()
	{
		gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);
		vs_out.texCoord = in_texCoord;
		vs_out.color = in_color;
	}
)";

static const std::string simpleFragmentShader = R"(
	#version 450 core
	#line 28

	in VS_OUT
	{
		vec2 texCoord;
		vec4 color;
	} fs_in;

	layout(location = 0) out vec4 fragColor;

	uniform sampler2D tex;

	vec4 cubic(float v)
	{
		vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
		vec4 s = n * n * n;
		float x = s.x;
		float y = s.y - 4.0 * s.x;
		float z = s.z - 4.0 * s.y + 6.0 * s.x;
		float w = 6.0 - x - y - z;
		return vec4(x, y, z, w) * (1.0/6.0);
	}

	vec4 textureBicubic(sampler2D sampler, vec2 texCoords)
	{
	   vec2 texSize = textureSize(sampler, 0);
	   vec2 invTexSize = 1.0 / texSize;

	   texCoords = texCoords * texSize - 0.5;


		vec2 fxy = fract(texCoords);
		texCoords -= fxy;

		vec4 xcubic = cubic(fxy.x);
		vec4 ycubic = cubic(fxy.y);

		vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;

		vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
		vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;

		offset *= invTexSize.xxyy;

		vec4 sample0 = texture(sampler, offset.xz);
		vec4 sample1 = texture(sampler, offset.yz);
		vec4 sample2 = texture(sampler, offset.xw);
		vec4 sample3 = texture(sampler, offset.yw);

		float sx = s.x / (s.x + s.y);
		float sy = s.z / (s.z + s.w);

		return mix(
			mix(sample3, sample2, sx),
			mix(sample1, sample0, sx),
			sy);
	}

	void main()
	{
		vec3 texSample = textureBicubic(tex, fs_in.texCoord).rgb;
		vec3 color = texSample * fs_in.color.rgb;
		fragColor = vec4(color, smoothstep(0.0, 1.0, fs_in.color.a));
	}
)";

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 1920, 1080)
	{
	}

	void DemoApplication::init()
	{
		m_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)getWidth() / getHeight(), 0.01f, 50.0f);

		m_viewPosition.z = 1;

		m_simpleShader = std::make_unique<plgn::Shader>(simpleVertexShader, simpleFragmentShader);
		m_simpleShader->use();
		m_simpleShader->setUniform("projectionMatrix", m_projectionMatrix);
		
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 texCoord;
			glm::vec4 color;

			Vertex(const glm::vec3& pos, const glm::vec2& tc, const glm::vec4& col)
				: position(pos), texCoord(tc), color(col) { }
		};
		constexpr int segments = 128;
		std::vector<Vertex> vertices;
		vertices.reserve((segments + 1) * 4);
		glm::vec4 color;
		color.a = 0.0f;
		//color.r = 142 / 255.0f;
		//color.g = 41 / 255.0f;
		//color.b = 109 / 255.0f;
		//color.r = 0.0f;
		//color.g = -1.0f;
		//color.b = -2.0f;
		std::srand((unsigned int)std::time(nullptr));
		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
				vertices.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 0.5f), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
			else if (i == 1)
				vertices.emplace_back(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.5f, 0.5f), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
			else if (i == 2)
				vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
			else if (i == 3)
				vertices.emplace_back(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));

			for (int j = 0; j < segments; j++)
			{
				float angle = (float)j / segments * glm::two_pi<float>();

				glm::vec3 position;
				glm::vec2 texCoord;
				if (i == 0)
				{
					position.x = glm::cos(angle);
					position.y = glm::sin(angle);
					position /= glm::max(glm::abs(position.x), glm::abs(position.y));
					position.z = 1.0f;
					texCoord = glm::vec2(position.x, position.y)* 0.5f + 0.5f;
				}
				else if (i == 1)
				{
					position.x = -glm::cos(angle);
					position.y = glm::sin(angle);
					position /= glm::max(glm::abs(position.x), glm::abs(position.y));
					position.z = -1.0f;
					texCoord = glm::vec2(position.x, position.y)* 0.5f + 0.5f;
				}
				else if (i == 2)
				{
					position.y = glm::sin(angle);
					position.z = glm::cos(angle);
					position /= glm::max(glm::abs(position.z), glm::abs(position.y));
					position.x = 1.0f;
					texCoord = glm::vec2(position.z, position.y)* 0.5f + 0.5f;
				}
				else if (i == 3)
				{
					position.y = glm::sin(angle);
					position.z = -glm::cos(angle);
					position /= glm::max(glm::abs(position.z), glm::abs(position.y));
					position.x = -1.0f;
					texCoord = glm::vec2(position.z, position.y)* 0.5f + 0.5f;
				}

				glm::vec4 color;
				color.r = (std::rand() % 256) / 255.0f;
				color.g = (std::rand() % 256) / 255.0f;
				color.b = (std::rand() % 256) / 255.0f;
				color.a = 0.0f;

				vertices.emplace_back(position, texCoord, color);
			}
		}
		std::vector<GLuint> indices;
		indices.reserve(segments * 3 * 4);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < segments; j++)
			{
				indices.push_back((segments + 1) * i);
				indices.push_back((segments + 1) * i + j + 1);
				indices.push_back((segments + 1) * i + (j == segments - 1 ? 1 : j + 2));
			}
		}
		m_numElements = indices.size();

		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		unsigned int texSize = 32;
		std::vector<unsigned char> pixels;
		pixels.reserve(texSize * texSize * 3);
		for (unsigned int y = 0; y < texSize; y++)
		{
			for (unsigned int x = 0; x < texSize; x++)
			{
				pixels.push_back((unsigned char)(((std::rand() % 256) * 75 / 100) + (256 * 25 / 100)));
				pixels.push_back((unsigned char)(((std::rand() % 256) * 75 / 100) + (256 * 25 / 100)));
				pixels.push_back((unsigned char)(((std::rand() % 256) * 75 / 100) + (256 * 25 / 100)));
				//pixels.push_back(pixels.back());
				//pixels.push_back(pixels.back());
				//pixels.push_back((unsigned char)(std::rand() % 256));
				//pixels.push_back((unsigned char)(std::rand() % 256));
			}
		}
		m_texture = std::make_unique<plgn::Texture2D>(texSize, texSize, plgn::TextureFormat::RGB_8, pixels.data());

		//glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void DemoApplication::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE))
		{
			stop();
			return;
		}

		float rotSpeed = (float)glm::radians(90.0 * deltaTime);
		float moveSpeed = (float)(0.5 * deltaTime);
		if (isKeyDown(GLFW_KEY_RIGHT))
		{
			m_rotation += rotSpeed;
		}
		if (isKeyDown(GLFW_KEY_LEFT))
		{
			m_rotation -= rotSpeed;
		}
		if (isKeyDown(GLFW_KEY_UP))
		{
			m_distance -= moveSpeed;
		}
		if (isKeyDown(GLFW_KEY_DOWN))
		{
			m_distance += moveSpeed;
		}

		m_viewPosition.x = (float)std::sin(m_rotation) * m_distance;
		m_viewPosition.z = (float)std::cos(m_rotation) * m_distance;
	}

	void DemoApplication::render()
	{
		//glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 viewMatrix = glm::lookAt(m_viewPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		m_simpleShader->use();
		m_simpleShader->setUniform("viewMatrix", viewMatrix);
		m_texture->bind();
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void DemoApplication::dispose()
	{
		m_simpleShader->destroy();
		m_simpleShader.reset();

		glDeleteVertexArrays(1, &m_vao); m_vao = 0;
		glDeleteBuffers(1, &m_vbo); m_vbo = 0;
		glDeleteBuffers(1, &m_ebo); m_ebo = 0;

		m_texture->destroy();
		m_texture.reset();
	}
}
