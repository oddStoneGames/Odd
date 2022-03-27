#pragma once

#include "Core.h"
#include "Window.h"
#include "Odd/Events/Event.h"
#include "Odd/Core/LayerStack.h"
#include "Odd/Events/ApplicationEvent.h"

namespace Odd
{
	class ODD_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();	//Defined By Client
}