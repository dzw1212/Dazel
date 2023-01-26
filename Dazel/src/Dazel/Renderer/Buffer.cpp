#include "DazelPCH.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace DAZEL
{
	Ref<VertexBuffer> VertexBuffer::Create(float* pfVertices, UINT uiSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL :
			{
				return std::make_shared<OpenGLVertexBuffer>(pfVertices, uiSize);
			}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(UINT uiSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLVertexBuffer>(uiSize);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(UINT* puiIndices, UINT uiCount)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(puiIndices, uiCount);
			}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(UINT uiCount)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(uiCount);
			}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
}