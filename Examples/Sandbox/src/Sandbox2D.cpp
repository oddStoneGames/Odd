#include <Odd.h>

class ExampleLayer : public Odd::Layer
{
public:
	ExampleLayer() : Layer("Example"){}

	void OnUpdate() override
	{
		//DEBUG_INFO("ExampleLayer::Update");
	}

	void OnEvent(Odd::Event& event) override
	{
		//DEBUG_TRACE("{0}", event);
	}
};

class Sandbox2D : public Odd::Application
{
	public:
		Sandbox2D() 
		{ 
			PushLayer(new ExampleLayer()); 
			PushOverlay(new Odd::ImGuiLayer());
		}
		~Sandbox2D() {}
};
 
Odd::Application* Odd::CreateApplication()
{
	return new Sandbox2D();
}