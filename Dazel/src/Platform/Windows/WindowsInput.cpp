#include "DazelPCH.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "Dazel/Core/Application.h"

namespace DAZEL
{
	Input* Input::s_Inst = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int nKeyCode)
	{
		auto& app = Application::Get();
		auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		int nState = glfwGetKey(window, nKeyCode);
		return nState == GLFW_PRESS || nState == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int nButton)
	{
		auto& app = Application::Get();
		auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		int nState = glfwGetMouseButton(window, nButton);
		return nState == GLFW_PRESS;
	}
	std::pair<float,float> WindowsInput::GetMousePosImpl()
	{
		auto& app = Application::Get();
		auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		double dPosX, dPosY;
		glfwGetCursorPos(window, &dPosX, &dPosY);
		return std::make_pair((float)dPosX, (float)dPosY); 
	}
}