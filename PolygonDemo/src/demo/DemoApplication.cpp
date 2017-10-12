#include "stdafx.h"

static const std::string simpleVertexShader = R"(
	#version 450 core
	#line 18

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_color;

	out VS_OUT
	{
		vec3 color;
	} vs_out;

	uniform mat4 projectionMatrix = mat4(1.0);
	uniform mat4 viewMatrix = mat4(1.0);

	void main()
	{
		gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);
		vs_out.color = in_color;
	}
)";

static const std::string simpleFragmentShader = R"(
	#version 450 core
	#line 28

	in VS_OUT
	{
		vec3 color;
	} fs_in;

	layout(location = 0) out vec4 fragColor;

	void main()
	{
		vec3 color = fs_in.color;
		fragColor = vec4(color, 1.0);
	}
)";

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 640, 480)
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
			glm::vec2 position;
			glm::vec3 color;

			Vertex(const glm::vec2& pos, const glm::vec3& col) : position(pos), color(col) { }
		};
		constexpr int segments = 128;
		std::vector<Vertex> vertices;
		vertices.reserve(segments + 1);
		vertices.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 3.0f));
		glm::vec3 color;
		color.r = 142 / 255.0f;
		color.g = 41 / 255.0f;
		color.b = 109 / 255.0f;
		//color.r = 0.0f;
		//color.g = -1.0f;
		//color.b = -2.0f;
		std::srand((unsigned int)std::time(nullptr));
		for (int i = 0; i < segments; i++)
		{
			glm::vec2 position;
			position.x = (GLfloat)std::cos((double)i / segments * M_PI * 2) * 0.8f;
			position.y = (GLfloat)std::sin((double)i / segments * M_PI * 2) * 0.8f;

			color.r = (std::rand() % 256) / 255.0f;
			color.g = (std::rand() % 256) / 255.0f;
			color.b = (std::rand() % 256) / 255.0f;

			vertices.emplace_back(position, color);
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
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void DemoApplication::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE))
		{
			stop();
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
	}
}
