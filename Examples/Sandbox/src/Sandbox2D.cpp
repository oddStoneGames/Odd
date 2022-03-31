#include <Odd.h>
#include "ImGui/include/imgui.h"

class ExampleLayer : public Odd::Layer
{
public:
	ExampleLayer() : Layer("Example"){}

	void OnUpdate() override
	{
		if (Odd::Input::IsKeyPressed(Odd::Key::Tab))
			DEBUG_INFO("Tab key Pressed(poll)!");
		glm::vec2 hi = glm::vec2(2, 3);
		//DEBUG_INFO("ExampleLayer::Update");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("haha");
		ImGui::Text("uwu");
		ImGui::End();
	}

	void OnEvent(Odd::Event& event) override
	{
		if (Odd::Input::IsKeyPressed(Odd::Key::Tab))
			DEBUG_INFO("Tab key Pressed(event)!");
		//DEBUG_TRACE("{0}", event);
	}
};

class Sandbox2D : public Odd::Application
{
	public:
		Sandbox2D() 
		{ 
			PushLayer(new ExampleLayer());
		}
		~Sandbox2D() {}
};
 
Odd::Application* Odd::CreateApplication()
{
	return new Sandbox2D();
}