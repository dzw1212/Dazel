#include "Dazel.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "EditorLayer.h"

///////////////////////Entry Point////////////////////////
#include "Dazel/Core/EntryPoint.h" //ע��include˳��EntryPoint��Ҫ�ú�

class EditorApp : public DAZEL::Application
{
public:
	EditorApp()
		: Application("Dazel Editor")
	{
		PushLayer(new EditorLayer());
	}

	~EditorApp()
	{

	}
};

DAZEL::Application* DAZEL::CreateApplication(ApplicationCommandLineArgs args)
{
	return new EditorApp();
}