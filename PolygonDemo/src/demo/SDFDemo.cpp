#include "stdafx.h"

#include <include/demo/SDFDemo.h>

static const std::string vertexSource = R"(
	#version 450 core
	#line 8

	out vec2 uv;

	const vec2 vertices[] = vec2[](
		vec2(-1, -1),
		vec2(1, -1),
		vec2(1, 1),
		vec2(-1, 1)
	);

	void main()
	{
		vec2 vert = vertices[(gl_VertexID - gl_VertexID / 3) % 4];
		gl_Position = vec4(vert, 0.0, 1.0);
		uv = vert;
	}
)";

static const std::string fragmentSource = R"(
	#version 450 core
	#line 29

	in vec2 uv;

	out vec4 fragColor;

	uniform float t;

	struct Intersect
	{
		float dist;
		vec3 col;
	};

	const vec2 circleCenter1 = vec2(0.0);
	const float circleRadius1 = 0.3;
	const vec3 circleColor1 = vec3(1.0, 0.0, 0.0);

	vec2 circleCenter2 = vec2(0.25, 0.0);
	const float circleRadius2 = 0.2;
	const vec3 circleColor2 = vec3(0.0, 0.0, 1.0);

	Intersect sdfCircle(vec2 c, float r, vec3 col, vec2 p)
	{
		return Intersect(distance(c, p) - r, col);
	}

	Intersect sdfMin(Intersect d1, Intersect d2, float k)
	{
		float res = exp(-k * d1.dist) + exp(-k * d2.dist);
		float d = -log(max(0.0001, res)) / k;
		vec3 c = mix(d1.col, d2.col, smoothstep(d1.dist, d2.dist, d));
		return Intersect(d, c);
	}

	void main()
	{
		circleCenter2 = vec2(cos(t), sin(t)*0) * 0.55;

		vec3 color = vec3(1.0);
		Intersect dist = sdfCircle(circleCenter1, circleRadius1, circleColor1, uv);
		dist = sdfMin(dist, sdfCircle(circleCenter2, circleRadius2, circleColor2, uv), 64);
		color = dist.col;
		float alpha = 1.0 - smoothstep(0.0, fwidth(dist.dist), dist.dist);
		fragColor = vec4(color, alpha);
	}
)";

namespace demo
{
	SDFDemo::SDFDemo() : Application("Polygon Engine SDF Demo", 1280, 720, true)
	{
	}

	void SDFDemo::init()
	{
		m_sdfShader = std::make_unique<plgn::Shader>(vertexSource, fragmentSource);
		m_screenVao = std::make_unique<plgn::VertexArray>(6);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SDFDemo::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE)) {
			stop();
		}
	}

	void SDFDemo::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_sdfShader->use();
		m_sdfShader->setUniform("t", (float)glfwGetTime());
		m_screenVao->draw();
	}

	void SDFDemo::dispose()
	{
		m_sdfShader->destroy();
	}
}
