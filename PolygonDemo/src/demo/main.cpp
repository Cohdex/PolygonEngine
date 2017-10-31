#include "stdafx.h"

#include <demo/DemoApplication.h>
#include <demo/SDFDemo.h>

int main(void)
{
	plgn::Application* app;
	app = new demo::DemoApplication();
	//app = new demo::SDFDemo();
	try
	{
		app->start();
	}
	catch (...) { }
	delete app;
	std::cin.get();
	return 0;
}
