#include "stdafx.h"

int main(void)
{
	plgn::Application* app = new demo::DemoApplication();
	app->start();
	delete app;
	std::cin.get();
	return 0;
}
