#include "stdafx.h"

#include <polygon/app/Application.h>

namespace plgn
{
	Application::Application(const std::string& title, int width, int height)
	{
		appLoop();
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

		glfwTerminate();
	}

	void Application::stop()
	{
		m_running = false;
	}
}
