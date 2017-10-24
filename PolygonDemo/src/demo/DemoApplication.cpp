#include "stdafx.h"

#include <demo/DemoApplication.h>

static const std::string simpleVertexShader = R"(
	#version 450 core
	#line 18

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_normal;
	layout(location = 2) in vec2 in_texCoord;

	out VS_OUT
	{
		vec3 position;
		vec3 normal;
		vec2 texCoord;
	} vs_out;

	uniform mat4 modelMatrix = mat4(1.0);
	uniform mat3 normalMatrix = mat3(1.0);
	uniform mat4 viewMatrix = mat4(1.0);
	uniform mat4 projectionMatrix = mat4(1.0);

	void main()
	{
		vec4 worldPos = modelMatrix * vec4(in_position, 1.0);
		gl_Position = projectionMatrix * viewMatrix * worldPos;

		vs_out.position = worldPos.xyz;
		vs_out.normal = normalize(normalMatrix * in_normal);
		vs_out.texCoord = in_texCoord;
	}
)";

static const std::string simpleFragmentShader = R"(
	#version 450 core
	#line 28

	in VS_OUT
	{
		vec3 position;
		vec3 normal;
		vec2 texCoord;
	} fs_in;

	layout(location = 0) out vec4 out_color;

	uniform sampler2D tex;
	uniform vec3 materialColor;

	uniform vec3 viewPosition;
	uniform float t;

	const vec3 directionalLight = normalize(vec3(-1, -2, -1));
	const vec3 lightColor = vec3(1.0, 1.0, 1.0);

	const float gamma = 2.2;

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

		return pow(mix(
			mix(sample3, sample2, sx),
			mix(sample1, sample0, sx),
			sy), vec4(gamma));
	}

	void main()
	{
		vec3 normal = normalize(mix(fs_in.normal, -fs_in.normal, step(1, int(!gl_FrontFacing))));

		vec3 texSample = textureBicubic(tex, fs_in.texCoord + t * 0.1).rgb;
		vec3 albedo = texSample * materialColor;

		vec3 ambient = albedo * lightColor * 0.04;

		vec3 diffuse = albedo * lightColor * max(0.0, dot(normal, -directionalLight)) * 0.8;

		vec3 viewDir = normalize(viewPosition - fs_in.position);
		vec3 specular =	lightColor * pow(max(0.0, dot(viewDir, reflect(directionalLight, normal))), 64) * 1.0;

		vec3 color = ambient + diffuse + specular;

		color += vec3(pow(1.0 - max(0.0, dot(normalize(viewPosition - fs_in.position), normal)), 8));

		out_color = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
	}
)";

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 1280, 720)
	{
	}

	void DemoApplication::init()
	{
		m_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)getWidth() / getHeight(), 0.01f, 50.0f);

		m_simpleShader = std::make_unique<plgn::Shader>(simpleVertexShader, simpleFragmentShader);
		m_simpleShader->use();
		m_simpleShader->setUniform("projectionMatrix", m_projectionMatrix);
		
		m_torusMesh.reset(plgn::MeshUtil::createTorus(0.75f, 0.25f, 128, 64));
		m_teapotMesh.reset(plgn::ObjLoader::load(RES_PATH "teapot.obj"));
		m_airplaneMesh.reset(plgn::ObjLoader::load(RES_PATH "f16.obj"));
		
		unsigned int texSize = 32;
		std::vector<unsigned char> pixels;
		pixels.reserve(texSize * texSize * 3);
		for (unsigned int y = 0; y < texSize; y++)
		{
			for (unsigned int x = 0; x < texSize; x++)
			{
				pixels.push_back((unsigned char)(((std::rand() % 256) * 75 / 100) + (255 * 25 / 100)));
				pixels.push_back(pixels.back());
				pixels.push_back(pixels.back());
			}
		}
		m_texture = std::make_unique<plgn::Texture2D>(texSize, texSize, plgn::TextureFormat::RGB_8, pixels.data());

		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	}

	void DemoApplication::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE))
		{
			stop();
			return;
		}

		float rotSpeed = (float)glm::radians(45.0 * deltaTime);
		float moveSpeed = (float)(0.5 * deltaTime);

		if (isKeyDown(GLFW_KEY_LEFT_SHIFT) || isKeyDown(GLFW_KEY_RIGHT_SHIFT))
			moveSpeed *= 3;

		if (isKeyDown(GLFW_KEY_RIGHT))
			m_camera.getYaw() += rotSpeed;
		if (isKeyDown(GLFW_KEY_LEFT))
			m_camera.getYaw() -= rotSpeed;
		if (isKeyDown(GLFW_KEY_UP))
			m_camera.getPitch() += rotSpeed;
		if (isKeyDown(GLFW_KEY_DOWN))
			m_camera.getPitch() -= rotSpeed;

		m_camera.getYaw() += (float)(getMouseScreenDX() * rotSpeed * 0.2);
		m_camera.getPitch() += (float)(getMouseScreenDY() * rotSpeed * 0.2);

		m_camera.getPitch() = glm::clamp(m_camera.getPitch(), glm::radians(-90.0f), glm::radians(90.0f));

		glm::vec3 movement;
		if (isKeyDown(GLFW_KEY_D))
			movement.x += moveSpeed;
		if (isKeyDown(GLFW_KEY_A))
			movement.x -= moveSpeed;
		if (isKeyDown(GLFW_KEY_W))
			movement.z += moveSpeed;
		if (isKeyDown(GLFW_KEY_S))
			movement.z -= moveSpeed;
		if (isKeyDown(GLFW_KEY_SPACE))
			movement.y += moveSpeed;
		if (isKeyDown(GLFW_KEY_C))
			movement.y -= moveSpeed;

		m_camera.move(movement);
	}

	void DemoApplication::render()
	{
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glm::vec3 viewTarget(0.0f, 0.0f, 0.0f);
		glm::mat4 viewMatrix = m_camera.getViewMatrix();
		glm::mat4 modelMatrix;

		m_simpleShader->use();
		m_simpleShader->setUniform("viewMatrix", viewMatrix);
		m_simpleShader->setUniform("viewPosition", m_camera.getPosition());
		m_simpleShader->setUniform("t", (float)glfwGetTime());
		m_texture->bind();
		
		modelMatrix = glm::translate(glm::vec3(-1.25f, -0.25f, -1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		m_simpleShader->setUniform("modelMatrix", modelMatrix);
		m_simpleShader->setUniform("normalMatrix", glm::mat3(1.0f));
		m_simpleShader->setUniform("materialColor", glm::vec3(30, 100, 255) / 255.0f);
		m_teapotMesh->draw();

		modelMatrix = glm::translate(glm::vec3(-1, 0, 1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
		m_simpleShader->setUniform("modelMatrix", modelMatrix);
		m_simpleShader->setUniform("normalMatrix", glm::mat3(1.0f));
		m_simpleShader->setUniform("materialColor", glm::vec3(0.05, 1.0, 0.2));
		m_airplaneMesh->draw();

		m_simpleShader->setUniform("modelMatrix", glm::mat4(1.0f));
		m_simpleShader->setUniform("normalMatrix", glm::mat3(1.0f));
		m_simpleShader->setUniform("materialColor", glm::vec3(1.0, 0.02, 0.02));
		m_torusMesh->draw();

		modelMatrix = glm::translate(glm::vec3(0.75f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
		m_simpleShader->setUniform("modelMatrix", modelMatrix);
		m_simpleShader->setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(modelMatrix)));
		m_simpleShader->setUniform("materialColor", glm::vec3(0.02, 0.02, 1.0));
		m_torusMesh->draw();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}

	void DemoApplication::dispose()
	{
		m_torusMesh->destroy();
		m_teapotMesh->destroy();
		m_airplaneMesh->destroy();

		m_simpleShader->destroy();

		m_texture->destroy();
	}
}
