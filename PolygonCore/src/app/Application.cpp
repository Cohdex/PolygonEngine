#include "stdafx.h"

#include "../../include/app/Application.h"

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
		update(deltaTime);
		if (m_running)
		{
			render();
			glfwSwapBuffers(m_window);
		}
	}

	glfwTerminate();
}
