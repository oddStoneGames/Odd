#pragma once

#include "Core.h"
#include "Window.h"
#include "Odd/Events/Event.h"
#include "Odd/Core/LayerStack.h"
#include "Odd/Events/ApplicationEvent.h"
#include "Odd/ImGui/ImGuiLayer.h"

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

			inline static Application& Get() { return *s_Instance; }
			inline Window& GetWindow() { return *m_Window; }
	private:
		static Application* s_Instance;
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();	//Defined By Client
}