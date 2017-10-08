#pragma once

#include <string>

namespace plgn
{
	class Application
	{
	private:
		std::string m_title;
		int m_width;
		int m_height;
		GLFWwindow* m_window;
		bool m_running = false;

		void createWindow();
		void appLoop();

	protected:
		Application(const std::string& title, int width, int height);

		virtual void init() = 0;
		virtual void update(double deltaTime) = 0;
		virtual void render() = 0;

	public:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void start();
		void stop();
	};
}
