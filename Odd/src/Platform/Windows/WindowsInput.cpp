#include "oddpch.h"
#include "Odd/Input/Input.h"

#include "Odd/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Odd {

	bool Odd::Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Odd::Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Odd::Input::GetMousePosition()
	{

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xP, yP;
		glfwGetCursorPos(window, &xP, &yP);
		return std::pair<float, float>(xP, yP);
	}

	float Odd::Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return (float)x;
	}

	float Odd::Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return (float)y;
	}

}