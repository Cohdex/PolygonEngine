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
		vec3 color = textureBicubic(tex, fs_in.texCoord).rgb;
		color *= fs_in.color.rgb;
		fragColor = vec4(color, sqrt(fs_in.color.a));
	}
)";

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 1920, 1080)
	{
	}

	void DemoApplication::init()
	{
		float pw = getWidth() / (float)glm::min(getWidth(), getHeight());
		float ph = getHeight() / (float)glm::min(getWidth(), getHeight());
		m_projectionMatrix = glm::ortho(-pw, pw, -ph, ph, 1.0f, -1.0f);

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
		vertices.reserve(segments + 1);
		vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
		glm::vec4 color;
		color.r = 142 / 255.0f;
		color.g = 41 / 255.0f;
		color.b = 109 / 255.0f;
		color.a = 0.0f;
		//color.r = 0.0f;
		//color.g = -1.0f;
		//color.b = -2.0f;
		std::srand((unsigned int)std::time(nullptr));
		for (int i = 0; i < segments; i++)
		{
			glm::vec3 position;
			position.x = (GLfloat)std::cos((double)i / segments * M_PI * 2) * 0.8f;
			position.y = (GLfloat)std::sin((double)i / segments * M_PI * 2) * 0.8f;
			position.z = 0.0f;

			glm::vec2 texCoord;
			texCoord = glm::vec2(position.x, position.y) / 0.8f * 0.5f + 0.5f;

			color.r = (std::rand() % 256) / 255.0f;
			color.g = (std::rand() % 256) / 255.0f;
			color.b = (std::rand() % 256) / 255.0f;

			vertices.emplace_back(position, texCoord, color);
		}
		std::vector<GLuint> indices;
		indices.reserve(segments * 3);
		for (int i = 0; i < segments; i++)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i == segments - 1 ? 1 : i + 2);
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
				pixels.push_back((unsigned char)(std::rand() % 192) + 64);
				pixels.push_back(pixels.back());
				pixels.push_back(pixels.back());
				//pixels.push_back((unsigned char)(std::rand() % 256));
				//pixels.push_back((unsigned char)(std::rand() % 256));
			}
		}
		m_texture = std::make_unique<plgn::Texture2D>(texSize, texSize, 3, pixels.data());

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

		float moveSpeed = (float)(0.5 * deltaTime);
		if (isKeyDown(GLFW_KEY_RIGHT))
		{
			m_viewPosition.x += moveSpeed;
		}
		if (isKeyDown(GLFW_KEY_LEFT))
		{
			m_viewPosition.x -= moveSpeed;
		}
		if (isKeyDown(GLFW_KEY_UP))
		{
			m_viewPosition.y += moveSpeed;
		}
		if (isKeyDown(GLFW_KEY_DOWN))
		{
			m_viewPosition.y -= moveSpeed;
		}
	}

	void DemoApplication::render()
	{
		//glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_simpleShader->use();
		m_simpleShader->setUniform("viewMatrix", glm::translate(glm::mat4(), -m_viewPosition));
		m_texture->bind();
		glBindVertexArray(m_vao);
		//glDrawArrays(GL_TRIANGLES, 0, m_numElements);
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
