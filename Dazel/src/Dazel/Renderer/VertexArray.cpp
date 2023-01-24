#include "DazelPCH.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace DAZEL
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLVertexArray>();
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
}