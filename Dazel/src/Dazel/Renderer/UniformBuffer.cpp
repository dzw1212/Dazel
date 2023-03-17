#include "DazelPCH.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace DAZEL
{
	Ref<UniformBuffer> UniformBuffer::Create(UINT uiSize, UINT uiBinding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLUniformBuffer>(uiSize, uiBinding);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
}