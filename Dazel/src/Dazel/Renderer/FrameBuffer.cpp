#include "DazelPCH.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace DAZEL
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
		}
		CORE_ASSERT(false, std::format("RendererAPI {0} is currently not support", (int)Renderer::GetAPI()));
		return nullptr;
	}
}