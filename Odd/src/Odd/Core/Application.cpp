#include "oddpch.h"
#include "Application.h"
#include "Odd/Events/ApplicationEvent.h"

namespace Odd
{
	Application::Application()
	{
		//Initialize Logger.
		Odd::Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}