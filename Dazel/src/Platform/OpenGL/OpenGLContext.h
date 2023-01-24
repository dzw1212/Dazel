#pragma once

#include "Dazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace DAZEL
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}