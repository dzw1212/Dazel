#include "DazelPCH.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace DAZEL
{
	Ref<Texture2D> Texture2D::Create(const std::string& strPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(strPath);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(UINT uiWidth, UINT uiHeight)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(uiWidth, uiHeight);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}
}