#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

const char* simpleVertexShader = R"(
	#version 450 core
	#line 9

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_color;

	out VS_OUT
	{
		vec3 color;
	} vs_out;

	void main()
	{
		gl_Position = vec4(in_position, 1.0);
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
		fragColor = vec4(1.0 - fs_in.color, 1.0);
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

int main(void)
{
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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Polygon Engine", nullptr, nullptr);
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

	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

	GLuint simpleShader = createShader(simpleVertexShader, simpleFragmentShader);
	if (simpleShader == 0)
	{
		glfwTerminate();
		return 1;
	}

	GLfloat vertices[]{
		+0.0f, +0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		+0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	double lastTime = glfwGetTime();
	double timer = 1.0;
	unsigned int fps = 0;
	while (!glfwWindowShouldClose(window))
	{
		const double time = glfwGetTime();
		const double deltaTime = time - lastTime;
		lastTime = time;

		fps++;
		timer -= deltaTime;
		if (timer <= 0)
		{
			std::cout << "FPS: " << fps << " / " << (1.0 / deltaTime) << std::endl;
			timer += 1.0;
			fps = 0;
		}

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(simpleShader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
