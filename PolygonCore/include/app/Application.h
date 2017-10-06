#include "stdafx.h"

#pragma once

class Application
{
private:
	GLFWwindow* window;

protected:
	Application(const std::string& title, int width, int height);

	virtual void init() = 0;
	virtual void update(double deltaTime) = 0;
	virtual void render() = 0;

public:
	Application(const Application&) = delete;
	Application(const Application&&) = delete;
	Application& operator=(const Application&) = delete;
};
