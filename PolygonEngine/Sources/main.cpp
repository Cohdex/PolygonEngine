#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		std::cin.get();
		return -1;
	}

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
		return -1;
	}

	glEnable(GL_MULTISAMPLE);

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
			std::cout << "FPS: " << fps << std::endl;
			timer += 1.0;
			fps = 0;
		}

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER | GL_STENCIL_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(0.0f, 0.5f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(-0.5f, -0.5f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(0.5f, -0.5f);
		}
		glEnd();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
