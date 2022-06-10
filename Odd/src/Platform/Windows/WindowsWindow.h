#pragma once

#include "oddpch.h"
#include "Odd/Core/Window.h"
#include "Odd/Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Odd/Events/ApplicationEvent.h"
#include "Odd/Events/MouseEvent.h"
#include "Odd/Events/KeyEvent.h"
#include <GLFW/glfw3.h>

#include "Odd/Renderer/OrthographicCamera.h"

namespace Odd {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() const { return m_Window; }
		GLFWwindow* GetGLFWwindow() { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		OrthographicCamera m_Camera;
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		int m_frameCount;
		float m_frameTime;
		float m_timeSinceStartup;
		float m_prevTimeSinceStartup;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}