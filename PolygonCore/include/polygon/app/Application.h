#pragma once

#include <string>
#include <unordered_set>

namespace plgn
{
	class Application
	{
		friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		std::string m_title;
		int m_width;
		int m_height;
		GLFWwindow* m_window;
		bool m_running = false;
		std::unordered_set<int> m_pressedKeys;

		void createWindow();
		void appLoop();

	protected:
		Application(const std::string& title, int width, int height);

		virtual void init() = 0;
		virtual void update(double deltaTime) = 0;
		virtual void render() = 0;
		virtual void dispose() = 0;

	public:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void start();
		void stop();

		bool isKeyDown(int key) const;
		bool wasKeyPressed(int key) const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
	};
}
