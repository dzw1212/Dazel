#include "DazelPCH.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"

namespace DAZEL
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& strPath)
		: m_strPath(strPath)
	{
		PROFILE_FUNCTION();

		int nWidth, nHeight, nChannel;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* imageData = nullptr;
		{
			PROFILE_SCOPE("stbi_load in OpenGLTexture2D(const std::string&)");
			imageData = stbi_load(strPath.c_str(), &nWidth, &nHeight, &nChannel, 0);
			CORE_ASSERT(imageData, "Failed to load image");
		}

		LOG_INFO("Load Image[{3}], width = {0}, height = {1}, channels = {2}", nWidth, nHeight, nChannel, strPath);

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (nChannel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (nChannel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		CORE_ASSERT(internalFormat & dataFormat, "Unsupport image format");

		m_uiWidth = nWidth;
		m_uiHeight = nHeight;
		m_eInternalFormat = internalFormat;
		m_eDataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_uiId);
		glTextureStorage2D(m_uiId, 1, m_eInternalFormat, m_uiWidth, m_uiHeight);
		glTextureParameteri(m_uiId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_uiId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_uiId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_uiId, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_uiId, 0, 0, 0, m_uiWidth, m_uiHeight, m_eDataFormat, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		
	}
	OpenGLTexture2D::OpenGLTexture2D(UINT uiWidth, UINT uiHeight)
		: m_uiWidth(uiWidth), m_uiHeight(uiHeight), m_eInternalFormat(GL_RGBA8), m_eDataFormat(GL_RGBA)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(((uiWidth > 0) && (uiHeight > 0)), "Create 2D texture failed, width:{}, height:{}", uiWidth, uiHeight);

		LOG_INFO("Create Image, width = {0}, height = {1}", uiWidth, uiHeight);

		CORE_ASSERT(m_eInternalFormat & m_eDataFormat, "Unsupport image format");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_uiId);
		glTextureStorage2D(m_uiId, 1, m_eInternalFormat, m_uiWidth, m_uiHeight);
		glTextureParameteri(m_uiId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_uiId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_uiId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_uiId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PROFILE_FUNCTION();

		glDeleteTextures(1, &m_uiId);
	}
	void OpenGLTexture2D::SetData(void* data, UINT uiSize)
	{
		PROFILE_FUNCTION();

		int nChannel = m_eInternalFormat == GL_RGBA8 ? 4 : 3;
		UINT uiTextureSize = m_uiWidth * m_uiHeight * nChannel;
		CORE_ASSERT(uiSize == uiTextureSize, "Texture set data filed, texture size:{}, data size:{}", uiTextureSize, uiSize);
		glTextureSubImage2D(m_uiId, 0, 0, 0, m_uiWidth, m_uiHeight, m_eDataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::Bind(const int nSlot) const
	{
		PROFILE_FUNCTION();

		glBindTextureUnit(nSlot, m_uiId);
	}
	UINT OpenGLTexture2D::GetId() const
	{
		return m_uiId;
	}
}