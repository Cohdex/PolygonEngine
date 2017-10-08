#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <polygon/app/Application.h>

#include <iostream>
#include <vector>

#include <cstdlib>
#include <ctime>

const char* simpleVertexShader = R"(
	#version 450 core
	#line 18

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_color;

	out VS_OUT
	{
		vec3 color;
	} vs_out;

	uniform float t;

	void main()
	{
		float c = cos(t);
		float s = sin(t);
		mat3 m = mat3(
			 c,  s,  0,
			-s,  c,  0,
			 0,  0,  1
		);
		gl_Position = vec4(m * in_position, 1.0);
		vs_out.color = in_color;
	}
)";

const char* simpleFragmentShader = R"(
	#version 450 core
	#line 28

	in VS_OUT
	{
		vec3 color;
	} fs_in;

	layout(location = 0) out vec4 fragColor;

	void main()
	{
		vec3 color = sqrt(fs_in.color);
		fragColor = vec4(color, 1.0);
	}
)";

GLuint createShader(const char* vertexSource, const char* fragmentSource)
{
	GLint status;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(vertexShader, logSize, nullptr, log.data());
		glDeleteShader(vertexShader);
		std::cout << "Failed to compile vertex shader:\n" << log.data() << std::endl;
		return 0;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(fragmentShader, logSize, nullptr, log.data());
		glDeleteShader(vertexShader);
		glDeleteProgram(fragmentShader);
		std::cout << "Failed to compile fragment shader:\n" << log.data() << std::endl;
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteProgram(fragmentShader);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetProgramInfoLog(program, logSize, nullptr, log.data());
		glDeleteProgram(program);
		std::cout << "Failed to compile fragment shader:\n" << log.data() << std::endl;
		return 0;
	}

	return program;
}

class DemoApp : public plgn::Application
{
public:
	DemoApp() : Application("Demo Application", 1920, 1080)
	{
	}

private:
	void init() override
	{
		std::cout << "init()" << std::endl;
	}

	void update(double deltaTime) override
	{
		std::cout << "update()" << std::endl;
	}

	void render() override
	{
		std::cout << "render()" << std::endl;
	}
};

int main(void)
{
	plgn::Application* app = new DemoApp();
	app->start();
	return 0;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		std::cin.get();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(1080, 1080, "Polygon Engine", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Failed to create window" << std::endl;
		std::cin.get();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cout << "Failed to initialize GLEW" << std::endl;
		std::cin.get();
		return -2;
	}

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_MULTISAMPLE);

	auto windowResizeCallback = [](GLFWwindow* window, int newWidth, int newHeight) {
		glViewport(0, 0, newWidth, newHeight);
	};
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

	GLuint simpleShader = createShader(simpleVertexShader, simpleFragmentShader);
	if (simpleShader == 0)
	{
		glfwTerminate();
		std::cin.get();
		return 1;
	}

	std::vector<GLfloat> vertices;
	float r = 244.0f / 255.0f;
	float g = 73.0f / 255.0f;
	float b = 196.0f / 255.0f;
	std::srand((unsigned int)std::time(nullptr));
	constexpr int segments = 256;
	for (int i = 0; i < segments; i++)
	{
		//r = (std::rand() % 256) / 255.0f;
		//g = (std::rand() % 256) / 255.0f;
		//b = (std::rand() % 256) / 255.0f;
		GLfloat x0 = (GLfloat)std::cos((double)i / segments * M_PI * 2) * 0.7f;
		GLfloat y0 = (GLfloat)std::sin((double)i / segments * M_PI * 2) * 0.7f;
		GLfloat x1 = (GLfloat)std::cos((double)(i + 1) / segments * M_PI * 2) * 0.7f;
		GLfloat y1 = (GLfloat)std::sin((double)(i + 1) / segments * M_PI * 2) * 0.7f;

		vertices.push_back(x0);
		vertices.push_back(y0);
		vertices.push_back(r);
		vertices.push_back(g);
		vertices.push_back(b);

		vertices.push_back(x1);
		vertices.push_back(y1);
		vertices.push_back(r);
		vertices.push_back(g);
		vertices.push_back(b);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	double rotation = 0;
	double speed = 0;

	double timePassed = 0.0;
	unsigned int fps = 0;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		const double time = glfwGetTime();
		const double deltaTime = time - lastTime;
		lastTime = time;

		fps++;
		timePassed += deltaTime;
		if (timePassed >= 1.0)
		{
			std::cout << "FPS: " << fps << " | frame time: " << (deltaTime * 1000.0) << "ms" << std::endl;
			timePassed -= (int) timePassed;
			fps = 0;
		}

		glfwPollEvents();

		speed += (std::rand() % 3) - 1;
		rotation += /*speed */ deltaTime;

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(simpleShader);
		glUniform1f(glGetUniformLocation(simpleShader, "t"), (float)rotation*0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
