#pragma once

#include "oddpch.h"
#include "Core.h"
#include "Odd/Events/Event.h"
#include "Window.h"

namespace Odd
{
	class ODD_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();	//Defined By Client
}