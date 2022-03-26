#include "oddpch.h"
#include "WindowsWindow.h"

namespace Odd {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DEBUG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			if (success > 0)
				DEBUG_CORE_INFO("GLFW Initialized Successfully!");
			else
				DEBUG_CORE_ERROR("Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		m_frameCount = 0;
		m_frameTime = 0.0f;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		s_GLFWInitialized = false;
		glfwTerminate();
		m_frameCount = 0;
		m_frameTime = 0.0f;
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_frameCount++;
		m_timeSinceStartup = glfwGetTime();
		m_frameTime = m_timeSinceStartup - m_prevTimeSinceStartup;
		m_prevTimeSinceStartup = m_timeSinceStartup;
		DEBUG_CORE_TRACE("Time Since Startup {0} Time Elapsed on Frame {1} is {2} ms", m_timeSinceStartup, m_frameCount, m_frameTime);
		glClearColor(sin(m_timeSinceStartup), cos(m_timeSinceStartup), 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}