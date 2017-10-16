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
			createWindow(false);
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

	void Application::createWindow(bool fullscreen)
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW" << std::endl;
			std::cin.get();
			throw -1;
		}

		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		GLFWmonitor* monitor = nullptr;
		if (fullscreen)
		{
			monitor = glfwGetPrimaryMonitor();
			m_width = vidmode->width;
			m_height = vidmode->height;
		}
		glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
		glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), monitor, nullptr);
		if (!m_window)
		{
			glfwTerminate();
			std::cout << "Failed to create window" << std::endl;
			std::cin.get();
			throw -1;
		}

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
		glfwSetCursorPosCallback(m_window, cursorCallback);
		glfwSetScrollCallback(m_window, scrollCallback);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		double xPos, yPos;
		glfwGetCursorPos(m_window, &xPos, &yPos);
		cursorCallback(m_window, m_width / 2.0, m_height / 2.0);

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
			m_mouseScreenDX = 0.0f;
			m_mouseScreenDY = 0.0f;
			m_verticalScrollDelta = 0.0f;
			m_horizontalScrollDelta = 0.0f;
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

	static void cursorCallback(GLFWwindow* window, double xPos, double yPos)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		float oldXPos = app->m_mouseScreenX;
		float oldYPos = app->m_mouseScreenY;
		app->m_mouseScreenX = (float)xPos;
		app->m_mouseScreenY = (float)(app->m_height - yPos - 1.0);

		app->m_mouseScreenDX = app->m_mouseScreenX - oldXPos;
		app->m_mouseScreenDY = app->m_mouseScreenY - oldYPos;
	}

	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		app->m_horizontalScrollDelta = (float)xOffset;
		app->m_verticalScrollDelta = (float)yOffset;
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
