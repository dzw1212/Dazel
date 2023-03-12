#pragma once
#include "Dazel/Renderer/FrameBuffer.h"

namespace DAZEL
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual FrameBufferSpecification& GetSpec() override { return m_Spec; }
		virtual const FrameBufferSpecification& GetSpec() const override { return m_Spec; }

		virtual const UINT GetColorAttachmentId(UINT uiIdx = 0) const override;
		virtual const UINT GetDepthStencilAttachmentId() const override { return m_uiDepthStencilAttachmentId; }

		void ClearFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(UINT uiWidth, UINT uiHeight) override;

		virtual int ReadPixel(int nAttachmentIdx, int nX, int nY) override;
		virtual void ClearColorAttachment(UINT uiIdx, int nValue) override;

		void Invalidate();
	private:
		FrameBufferSpecification m_Spec;
		UINT m_uiId = 0;
		
		std::vector<FrameBufferTextureSpecification> m_vecColorAttachmentSpec;
		FrameBufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<UINT> m_vecColorAttachmentId;
		UINT m_uiDepthStencilAttachmentId = 0;
	};
}