#pragma once

#include <string>
#include <unordered_set>

namespace plgn
{
	class Application
	{
		friend void resizeCallback(GLFWwindow* window, int width, int height);
		friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void cursorCallback(GLFWwindow* window, double xPos, double yPos);

	private:
		std::string m_title;
		int m_width;
		int m_height;
		GLFWwindow* m_window;
		bool m_running = false;
		std::unordered_set<int> m_pressedKeys;
		float m_mouseScreenX;
		float m_mouseScreenY;
		float m_mouseScreenDX;
		float m_mouseScreenDY;

		void createWindow(bool fullscreen);
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

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		void start();
		void stop();

		bool isKeyDown(int key) const;
		bool wasKeyPressed(int key) const;

		float getMouseScreenX() const { return m_mouseScreenX; }
		float getMouseScreenY() const { return m_mouseScreenY; }
		float getMouseScreenDX() const { return m_mouseScreenDX; }
		float getMouseScreenDY() const { return m_mouseScreenDY; }
		float getMouseX() const { return m_mouseScreenX / m_width; }
		float getMouseY() const { return m_mouseScreenY / m_height; }
		float getMouseDX() const { return m_mouseScreenDX / m_width; }
		float getMouseDY() const { return m_mouseScreenDY / m_height; }
	};
}
