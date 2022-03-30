#include "oddpch.h"
#include "WindowsInput.h"

#include "Odd/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Odd {

	Input* Input::s_Instance = new WindowsInput();

	bool Odd::WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Odd::WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Odd::WindowsInput::GetMousePositionImpl()
	{

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xP, yP;
		glfwGetCursorPos(window, &xP, &yP);
		return std::pair<float, float>(xP, yP);
	}

	float Odd::WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return (float)x;
	}

	float Odd::WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return (float)y;
	}

}