#include "stdafx.h"

#include <polygon/app/Application.h>

namespace plgn
{
	Application::Application(const std::string& title, int width, int height)
		: m_title(title), m_width(width), m_height(height)
	{
	}

	void Application::start()
	{
		try
		{
			createWindow();
			init();
			appLoop();
		}
		catch (int e)
		{
			glfwTerminate();
			std::cin.get();
			throw e;
		}
	}

	void Application::stop()
	{
		m_running = false;
	}

	void Application::createWindow()
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW" << std::endl;
			std::cin.get();
			throw -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		//m_window = glfwCreateWindow(vidmode->width, vidmode->height, m_title.c_str(), glfwGetPrimaryMonitor(), nullptr);
		if (!m_window)
		{
			glfwTerminate();
			std::cout << "Failed to create window" << std::endl;
			std::cin.get();
			throw -1;
		}

		glfwSetWindowUserPointer(m_window, this);
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			std::cout << "Failed to initialize GLEW" << std::endl;
			std::cin.get();
			throw -2;
		}

		glfwGetFramebufferSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);

		glEnable(GL_MULTISAMPLE);

		glfwSetWindowSizeCallback(m_window, resizeCallback);
		glfwSetKeyCallback(m_window, keyCallback);

		std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
	}

	void Application::appLoop()
	{
		m_running = true;
		double lastTime = glfwGetTime();
		while (m_running)
		{
			double time = glfwGetTime();
			double deltaTime = time - lastTime;
			lastTime = time;

			m_pressedKeys.clear();
			glfwPollEvents();

			if (glfwWindowShouldClose(m_window))
			{
				m_running = false;
			}

			if (m_running)
			{
				update(deltaTime);
			}

			if (m_running)
			{
				render();
				glfwSwapBuffers(m_window);
			}
		}

		dispose();

		glfwTerminate();
	}

	static void resizeCallback(GLFWwindow* window, int width, int height)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		app->m_width = width;
		app->m_height = height;
		glViewport(0, 0, width, height);
	}

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
		{
			app->m_pressedKeys.insert(key);
		}
	}

	bool Application::isKeyDown(int key) const
	{
		return glfwGetKey(m_window, key);
	}

	bool Application::wasKeyPressed(int key) const
	{
		return m_pressedKeys.count(key) == 1;
	}
}
