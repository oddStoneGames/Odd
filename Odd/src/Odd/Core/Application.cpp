#include "Application.h"

#include "Odd/Events/ApplicationEvent.h"
#include "Odd/Core/Log.h"

namespace Odd
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}
	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);
		DEBUG_INFO(e);

		while (true);
	}
}