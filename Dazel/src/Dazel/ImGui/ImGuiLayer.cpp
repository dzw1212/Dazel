#include "DazelPCH.h"
#include "ImGuiLayer.h"

#include "Platform/OpenGL/imgui_impl_opengl3.h"


#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace DAZEL
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		auto CurContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(CurContext);
		ImGui::StyleColorsClassic();

		auto& io = ImGui::GetIO();
		io.BackendFlags | ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags | ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
		auto& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)1920, (float)1080);
		io.DeltaTime = 1.f / 60.f;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		auto& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		auto& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		auto& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());

		return false;
	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		auto& io = ImGui::GetIO();
		io.MouseWheel += event.GetOffsetY();
		io.MouseWheelH += event.GetOffsetX();

		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		if (event.GetKeyCode() < 0 || event.GetKeyCode() >= ImGuiKey_COUNT)
		{
			CORE_LOG_ERROR("Unrecognized Keycode:{}", event.GetKeyCode());
			return false;
		}

		auto& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		if (event.GetKeyCode() < 0 || event.GetKeyCode() >= ImGuiKey_COUNT)
		{
			CORE_LOG_ERROR("Unrecognized Keycode:{}", event.GetKeyCode());
			return false;
		}

		auto& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;

		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		auto& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());

		io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());
		
		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		auto& io = ImGui::GetIO();
		if (event.GetKeyCode() > 0 && event.GetKeyCode() < 0x10000)
			io.AddInputCharacter((unsigned short)event.GetKeyCode());

		return false;
	}
}