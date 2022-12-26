#include <Dazel.h>

class ExampleLayer : public DAZEL::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer") {}

	void OnUpdate() override
	{

	}

	void OnEvent(DAZEL::Event& event) override
	{

	}
};

class SandBox : public DAZEL::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new DAZEL::ImGuiLayer());
	}

	~SandBox()
	{

	}


};

DAZEL::Application* DAZEL::CreateApplication()
{
	return new SandBox();
}