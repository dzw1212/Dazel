#pragma once

#include "Dazel/Renderer/Texture.h"

#include "glad/glad.h"

namespace DAZEL
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& strPath);
		OpenGLTexture2D(UINT uiWidth, UINT uiHeight);
		virtual ~OpenGLTexture2D();

		virtual UINT GetWidth() const override { return m_uiWidth; }
		virtual UINT GetHeight() const override { return m_uiHeight; }


		virtual void SetData(void* data, UINT uiSize) override;
		virtual void Bind(const int nSlot) const override;		
	private:
		std::string m_strPath;
		UINT m_uiWidth;
		UINT m_uiHeight;
		UINT m_uiId;

		GLenum m_eInternalFormat;
		GLenum m_eDataFormat;
	};
}