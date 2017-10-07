#pragma once

#include <string>

namespace plgn
{
	class Application
	{
	private:
		GLFWwindow* m_window;
		bool m_running = false;

		void appLoop();

	protected:
		Application(const std::string& title, int width, int height);

		virtual void init() = 0;
		virtual void update(double deltaTime) = 0;
		virtual void render() = 0;

	public:
		Application(const Application&) = delete;
		Application(const Application&&) = delete;
		Application& operator=(const Application&) = delete;

		void stop();
	};
}
