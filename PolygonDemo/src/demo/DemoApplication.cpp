#include "stdafx.h"

#include <demo/DemoApplication.h>

static const std::string simpleVertexShader = R"(
	#version 450 core
	#line 8

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_normal;
	layout(location = 2) in vec2 in_texCoord;
	layout(location = 3) in vec3 in_tangent;

	out VS_OUT
	{
		vec3 position;
		vec3 normal;
		vec2 texCoord;
		mat3 TBN;
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
		vec3 T = normalize(normalMatrix * in_tangent);
		vec3 N = vs_out.normal;
		vec3 B = cross(N, T);
		if (in_tangent == vec3(0.0))
		{
			T = vec3(0.0);
			B = vec3(0.0);
		}
		vs_out.TBN = mat3(T, B, N);
	}
)";

static const std::string simpleFragmentShader = R"(
	#version 450 core
	#line 45

	in VS_OUT
	{
		vec3 position;
		vec3 normal;
		vec2 texCoord;
		mat3 TBN;
	} fs_in;

	layout(location = 0) out vec4 out_color;

	uniform sampler2D tex;
	uniform vec3 materialColor;
	uniform sampler2D normalMap;

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

		return mix(
			mix(sample3, sample2, sx),
			mix(sample1, sample0, sx),
			sy);
	}

	vec3 getNormal(vec2 texCoord)
	{
		vec3 normal = texture(normalMap, texCoord).xyz * 2.0 - 1.0;
		return normalize(fs_in.TBN * normal);
	}

	void main()
	{
		vec2 texCoord = fs_in.texCoord + (texture(normalMap, fs_in.texCoord + t * 0.1).rg * 2.0 - 1.0) * 0.01;

		vec3 normal;
		#if 0
			normal = normalize(fs_in.normal);
		#else
			normal = mix(getNormal(texCoord), normalize(fs_in.normal), smoothstep(0.0, 1.0, sin(t) * 0.5 + 0.5));
		#endif
		normal = mix(normal, -normal, step(1, int(!gl_FrontFacing)));

		vec3 texSample = pow(texture(tex, texCoord).rgb, vec3(gamma));
		vec3 albedo = texSample * materialColor;

		vec3 ambient = albedo * lightColor * 0.04;

		vec3 diffuse = albedo * lightColor * max(0.0, dot(normal, -directionalLight)) * 0.8;

		vec3 viewDir = normalize(viewPosition - fs_in.position);
		vec3 specular =	lightColor * pow(max(0.0, dot(viewDir, reflect(directionalLight, normal))), 64) * 0.9;

		vec3 color = ambient + diffuse + specular;

		vec3 edgeColor = vec3(pow(1.0 - max(0.0, dot(normalize(viewPosition - fs_in.position), normal)), 16)) * materialColor;// * vec3(0.7, 0.0, 0.0);
		//color += edgeColor;

		//color = vec3((2 * 0.01) / (50.0 + 0.01 - gl_FragCoord.z * (50.0 - 0.01)));

		out_color = vec4(color, 1.0);//vec4(pow(color, vec3(1.0 / gamma)), 1.0);
	}
)";

static const std::string screenVertexShader = R"(
	#version 450 core
	#line 160

	const vec2 positions[] = vec2[](
		vec2(-1, -1),
		vec2(1, -1),
		vec2(-1, 1),
		vec2(1, 1)
	);

	out vec2 texCoord;

	void main()
	{
		vec2 pos = positions[gl_VertexID];
		gl_Position = vec4(pos, 0, 1);
		texCoord = pos * 0.5 + 0.5;
	}
)";

static const std::string screenFragmentShader = R"(
	#version 450 core
	#line 181

	in vec2 texCoord;

	out vec4 outColor;

	uniform sampler2D tex;

	const float gamma = 2.2;

	float gray(vec3 color)
	{
		return dot(color, vec3(0.2126, 0.7152, 0.0722));
	}

	vec3 convolute(vec3 samples[3][3], float kernel[9])
	{
		vec3 result = vec3(0.0);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result += samples[i][j] * kernel[i + j * 3];
			}
		}
		return result;
	}

	vec3[3][3] getSamples()
	{
		vec3 samples[3][3];
		samples[0][0] = textureOffset(tex, texCoord, ivec2(-1, -1)).rgb;
		samples[1][0] = textureOffset(tex, texCoord, ivec2(0, -1)).rgb;
		samples[2][0] = textureOffset(tex, texCoord, ivec2(1, -1)).rgb;
		samples[0][1] = textureOffset(tex, texCoord, ivec2(-1, 0)).rgb;
		samples[1][1] = textureOffset(tex, texCoord, ivec2(0, 0)).rgb;
		samples[2][1] = textureOffset(tex, texCoord, ivec2(1, 0)).rgb;
		samples[0][2] = textureOffset(tex, texCoord, ivec2(-1, 1)).rgb;
		samples[1][2] = textureOffset(tex, texCoord, ivec2(0, 1)).rgb;
		samples[2][2] = textureOffset(tex, texCoord, ivec2(1, 1)).rgb;
		return samples;
	}

	float sobel(vec3 samples[3][3])
	{
		float xKernel[9] = { // X-kernel
			-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1
		};
		float yKernel[9] = { // Y-kernel
			 1,  2,  1,
			 0,  0,  0,
			-1, -2, -1
		};

		float magX = gray(convolute(samples, xKernel));
		float magY = gray(convolute(samples, yKernel));

		return sqrt(magX*magX + magY*magY);
	}

	void main()
	{
		//vec3 color = texture(tex, texCoord).rgb;
		//color = vec3(dot(color, vec3(0.2126, 0.7152, 0.0722)));
		vec3 samples[3][3] = getSamples();
		//float kernel[9] = { // Drunk
		//	2,   2, 2,
		//	2, -15, 2,
		//	2,   2, 2
		//};
		//float kernel[9] = { // Blur
		//	1.0/16, 2.0/16, 1.0/16,
		//	2.0/16, 4.0/16, 2.0/16,
		//	1.0/16, 2.0/16, 1.0/16
		//};
		//float kernel[9] = { // Edge
		//	1, 1, 1,
		//	1, -8, 1,
		//	1, 1, 1
		//};
		float kernel[9] = { // Sharpen
			0, -1, 0,
			-1, 5, -1,
			0, -1, 0
		};
		//float kernel[9] = { // Emboss
		//	-2, -1, 0,
		//	-1, 1, 1,
		//	0, 1, 2
		//};
		//float kernel[9] = { // Y-Sobel
		//	1, 2, 1,
		//	0, 0, 0,
		//	-1, -2, -1
		//};
		vec3 color;// = convolute(getSamples(), kernel);
		color = vec3(sobel(samples));
		//color = vec3(gray(color));
		outColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
	}
)";

namespace demo
{
	static GLuint framebuffer;
	static GLuint textureAttachment0;
	std::unique_ptr<plgn::Shader> screenShader;
	std::unique_ptr<plgn::VertexArray> screenVao;

	DemoApplication::DemoApplication() : Application("Polygon Engine Demo Application", 1280, 720, false)
	{
	}

	void DemoApplication::init()
	{
		m_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)getWidth() / getHeight(), 0.01f, 50.0f);

		m_simpleShader = std::make_unique<plgn::Shader>(simpleVertexShader, simpleFragmentShader);
		m_simpleShader->use();
		m_simpleShader->setUniform("projectionMatrix", m_projectionMatrix);
		m_simpleShader->setUniform("tex", 0);
		m_simpleShader->setUniform("normalMap", 1);
		
		m_meshes["torus"]    = std::shared_ptr<plgn::VertexArray>(plgn::MeshUtil::createTorus(0.75f, 0.25f, 128, 64));
		m_meshes["plane"]    = std::shared_ptr<plgn::VertexArray>(plgn::MeshUtil::createPlane(1, 1, 50, 50));
		m_meshes["cube"]     = std::shared_ptr<plgn::VertexArray>(plgn::MeshUtil::createCube(1, 1, 1));
		m_meshes["teapot"]   = std::shared_ptr<plgn::VertexArray>(plgn::ObjLoader::load("res/models/teapot.obj"));
		m_meshes["airplane"] = std::shared_ptr<plgn::VertexArray>(plgn::ObjLoader::load("res/models/f16.obj"));
		m_meshes["spider"]   = std::shared_ptr<plgn::VertexArray>(plgn::ObjLoader::load("res/models/spider.obj"));

		//m_testModel = std::make_unique<plgn::Model>("res/models/teapot.obj");

		m_models.push_back({ m_meshes["plane"],
			glm::vec3(0, -0.25f, 0),
			glm::radians(glm::vec3(-90, 0, 0)),
			glm::vec3(100.0f),
			glm::vec3(0.2f, 0.3f, 0.4f)
		});
		m_models.push_back({ m_meshes["cube"],
			glm::vec3(1.5f, 0.75f, -2),
			glm::radians(glm::vec3(0, -30, 0)),
			glm::vec3(2.0f),
			glm::vec3(0.8f, 0.8f, 0.05f)
		});
		m_models.push_back({ m_meshes["torus"],
			glm::vec3(),
			glm::vec3(),
			glm::vec3(1.0f),
			glm::vec3(1.0f, 0.02f, 0.02f)
		});
		m_models.push_back({ m_meshes["torus"],
			glm::vec3(0.75f, 0, 0),
			glm::radians(glm::vec3(90, 0, 0)),
			glm::vec3(1.0f),
			glm::vec3(0.02f, 0.02f, 1.0f)
		});
		m_models.push_back({ m_meshes["teapot"],
			glm::vec3(-1.25f, -0.25f, -1),
			glm::vec3(),
			glm::vec3(0.1f),
			glm::vec3(30, 100, 255) / 255.0f
		});
		m_models.push_back({ m_meshes["airplane"],
			glm::vec3(-1, 0, 1.5f),
			glm::vec3(),
			glm::vec3(2),
			glm::vec3(0.05f, 1.0f, 0.2f)
		});
		m_models.push_back({ m_meshes["spider"],
			glm::vec3(-1.25f, -0.17f, -1),
			glm::vec3(),
			glm::vec3(0.002f),
			glm::vec3(0.2f)
		});
		
		unsigned int texSize = 32;
		std::vector<unsigned char> pixels;
		pixels.reserve(texSize * texSize * 3);
		for (unsigned int y = 0; y < texSize; y++)
		{
			for (unsigned int x = 0; x < texSize; x++)
			{
				pixels.push_back((unsigned char)(((std::rand() % 256) * 75 / 100) + (255 * 25 / 100)));
				pixels.back() = 0;
				pixels.push_back(pixels.back());
				pixels.push_back(pixels.back());
			}
		}
		//m_texture = std::make_unique<plgn::Texture2D>(texSize, texSize, plgn::TextureFormat::RGB_8, pixels.data());
		m_texture = std::make_unique<plgn::Texture2D>("res/textures/abstract.png");

		//m_normalMap = std::make_unique<plgn::Texture2D>("res/textures/marble_normal.png");
		m_normalMap = std::make_unique<plgn::Texture2D>("res/textures/rocks_normal.jpg");
		//m_normalMap = std::make_unique<plgn::Texture2D>("res/textures/alien_rocks_normal.jpg");

		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		{
			glGenTextures(1, &textureAttachment0);
			glBindTexture(GL_TEXTURE_2D, textureAttachment0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, getWidth(), getHeight(), 0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttachment0, 0);

			GLuint depthStencilBuffer;
			glGenRenderbuffers(1, &depthStencilBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Failed to create framebuffer" << std::endl;
			throw 1;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		screenShader = std::make_unique<plgn::Shader>(screenVertexShader, screenFragmentShader);
		screenVao = std::make_unique<plgn::VertexArray>(4, plgn::DrawMode::TRIANGLE_STRIP);
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

		if (isKeyDown(GLFW_KEY_LEFT_SHIFT))
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
			movement.x++;
		if (isKeyDown(GLFW_KEY_A))
			movement.x--;
		if (isKeyDown(GLFW_KEY_W))
			movement.z++;
		if (isKeyDown(GLFW_KEY_S))
			movement.z--;
		if (glm::length(movement) > 1)
			movement = glm::normalize(movement);
		if (isKeyDown(GLFW_KEY_SPACE))
			movement.y++;
		if (isKeyDown(GLFW_KEY_C))
			movement.y--;

		m_camera.move(movement * moveSpeed);
	}

	void DemoApplication::render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glm::vec3 viewTarget(0.0f, 0.0f, 0.0f);
		glm::mat4 viewMatrix = m_camera.getViewMatrix();

		m_simpleShader->use();
		m_simpleShader->setUniform("viewMatrix", viewMatrix);
		m_simpleShader->setUniform("viewPosition", m_camera.getPosition());
		m_simpleShader->setUniform("t", (float)glfwGetTime());
		m_texture->bind(0);
		m_normalMap->bind(1);
		
		for (const Model& m : m_models)
		{
			glm::mat4 modelMatrix = m.getModelMatrix();
			glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelMatrix));
			m_simpleShader->setUniform("modelMatrix", modelMatrix);
			m_simpleShader->setUniform("normalMatrix", normalMatrix);
			m_simpleShader->setUniform("materialColor", m.materialColor);
			m.mesh->draw();
		}

		//m_testModel->draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAttachment0);
		screenShader->use();
		screenVao->draw();
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
	}

	void DemoApplication::dispose()
	{
		m_simpleShader->destroy();

		m_texture->destroy();
		m_normalMap->destroy();

		for (auto it : m_meshes)
		{
			it.second->destroy();
		}
	}
}
