#include "DazelPCH.h"
#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"

namespace DAZEL
{
	static GLenum FrameBufferTextureFormatToOpenGLFormat(FrameBufferTextureFormat eFormat)
	{
		switch (eFormat)
		{
		case FrameBufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FrameBufferTextureFormat::RED_INTEGER:
			return GL_RED_INTEGER;
		default:
			CORE_ASSERT(false, "Unsupport Frame Buffer Texture Format");
			break;
		}
		return GL_RGBA8;
	}
	static bool IsDepthFormat(FrameBufferTextureFormat eFormat)
	{
		return eFormat == FrameBufferTextureFormat::DEPTH24STENCIL8;
	}

	GLenum TextureTarget(bool bMultiSampled)
	{
		return bMultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool bMultiSampled, UINT* outId, UINT uiCount)
	{
		glCreateTextures(TextureTarget(bMultiSampled), uiCount, outId);
	}

	static void BindTexture(bool bMultiSampled, UINT uiId)
	{
		glBindTexture(TextureTarget(bMultiSampled), uiId);
	}

	static void AttachColorTexture(UINT uiId, int nSample, GLenum eInternalFormat, GLenum eStorageFormat, UINT uiWidth, UINT uiHeight, int nIndex)
	{
		bool bMultiSampled = nSample > 1;
		if (bMultiSampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nSample, eInternalFormat, uiWidth, uiHeight, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, eInternalFormat, uiWidth, uiHeight, 0, eStorageFormat, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + nIndex, TextureTarget(bMultiSampled), uiId, 0);
	}

	static void AttachDepthTexture(UINT uiId, int nSample, GLenum eInternalFormat, GLenum eAttachmentType, UINT uiWidth, UINT uiHeight)
	{
		bool bMultiSampled = nSample > 1;
		if (bMultiSampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nSample, eInternalFormat, uiWidth, uiHeight, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, eInternalFormat, uiWidth, uiHeight);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, eAttachmentType, TextureTarget(bMultiSampled), uiId, 0);
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Spec(spec)
	{

		//默认Depth Attachment是最后一个，前面的都是Color Attachment
		for (auto attachment : m_Spec.AttachmentSpec.m_vecAttachment)
		{
			if (!IsDepthFormat(attachment.m_eTextureFormat))
			{
				m_vecColorAttachmentSpec.push_back(attachment);
			}
			else
			{
				m_DepthAttachmentSpec = attachment;
				break;
			}
		}


		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		ClearFrameBuffer();
	}

	const UINT OpenGLFrameBuffer::GetColorAttachmentId(UINT uiIdx) const
	{
		CORE_ASSERT(uiIdx < m_vecColorAttachmentId.size(), "Get color attachemnt Id out of range");
		return m_vecColorAttachmentId[uiIdx];
	}

	void OpenGLFrameBuffer::ClearFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_uiId);
		glDeleteTextures(m_vecColorAttachmentId.size(), m_vecColorAttachmentId.data());
		glDeleteTextures(1, &m_uiDepthStencilAttachmentId);

		m_vecColorAttachmentId.clear();
		m_uiDepthStencilAttachmentId = 0;
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiId);
		glViewport(0, 0, m_Spec.uiWidth, m_Spec.uiHeight);
	}
	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(UINT uiWidth, UINT uiHeight)
	{
		m_Spec.uiWidth = uiWidth;
		m_Spec.uiHeight = uiHeight;
		Invalidate();
	}
	int OpenGLFrameBuffer::ReadPixel(int nAttachmentIdx, int nX, int nY)
	{
		CORE_ASSERT(nAttachmentIdx < m_vecColorAttachmentId.size(), "Get color attachemnt idx out of range");

		int nPixelData = 0;

		Bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0 + nAttachmentIdx);
		glReadPixels(nX, nY, 1, 1, GL_RED_INTEGER, GL_INT, &nPixelData);
		Unbind();
		return nPixelData;
	}
	void OpenGLFrameBuffer::ClearColorAttachment(UINT uiIdx, int nValue)
	{
		CORE_ASSERT(uiIdx < m_vecColorAttachmentId.size(), "Get color attachemnt idx out of range");

		auto& spec = m_vecColorAttachmentSpec[uiIdx];
		glClearTexImage(m_vecColorAttachmentId[uiIdx], 0, FrameBufferTextureFormatToOpenGLFormat(spec.m_eTextureFormat), GL_INT, &nValue);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_uiId != 0)
		{
			ClearFrameBuffer();
		}

		glCreateFramebuffers(1, &m_uiId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiId);

		bool bMultiSampled = m_Spec.uiSample > 1;

		//Color
		UINT uiColorAttachmentNum = m_vecColorAttachmentSpec.size();
		if (uiColorAttachmentNum > 0)
		{
			m_vecColorAttachmentId.resize(uiColorAttachmentNum);
			CreateTextures(bMultiSampled, m_vecColorAttachmentId.data(), m_vecColorAttachmentId.size());

			for (UINT i = 0; i < uiColorAttachmentNum; ++i)
			{
				BindTexture(bMultiSampled, m_vecColorAttachmentId[i]);

				switch (m_vecColorAttachmentSpec[i].m_eTextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					AttachColorTexture(m_vecColorAttachmentId[i], m_Spec.uiSample, GL_RGBA8, GL_RGBA, m_Spec.uiWidth, m_Spec.uiHeight, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					//每个像素包含一个整数类型的红色分量
					AttachColorTexture(m_vecColorAttachmentId[i], m_Spec.uiSample, GL_R32I, GL_RED_INTEGER, m_Spec.uiWidth, m_Spec.uiHeight, i);
					break;
				default:
					CORE_ASSERT(false, "Not support color attachment format");
					break;
				}
			}
		}
		
		//Depth Stencil
		if (m_DepthAttachmentSpec.m_eTextureFormat != FrameBufferTextureFormat::NONE)
		{
			CreateTextures(bMultiSampled, &m_uiDepthStencilAttachmentId, 1);
			BindTexture(bMultiSampled, m_uiDepthStencilAttachmentId);

			switch (m_DepthAttachmentSpec.m_eTextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_uiDepthStencilAttachmentId, m_Spec.uiSample, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.uiWidth, m_Spec.uiHeight);
				break;
			default:
				CORE_ASSERT(false, "Not support depth attachment format");
				break;
			}
		}
		
		if (m_vecColorAttachmentSpec.size() >= 1)
		{
			CORE_ASSERT(m_vecColorAttachmentSpec.size() <= 4, "Frame Buffer Color Attachment Num > 4");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_vecColorAttachmentSpec.size(), buffers);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}