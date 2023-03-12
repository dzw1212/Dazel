#pragma once

#include "Dazel//Core/Core.h"

namespace DAZEL
{
	enum class FrameBufferTextureFormat
	{
		NONE = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		//Depth-Stencil
		DEPTH24STENCIL8,

		//Default
		DEFAULT_COLOR = RGBA8,
		DEFAULT_DEPTH = DEPTH24STENCIL8,
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat eFormat)
			: m_eTextureFormat(eFormat)
		{}

		FrameBufferTextureFormat m_eTextureFormat = FrameBufferTextureFormat::NONE;

		//todo : filtering/wrap
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: m_vecAttachment(attachments)
		{}

		std::vector<FrameBufferTextureSpecification> m_vecAttachment;
	};

	struct FrameBufferSpecification
	{
		UINT uiWidth = 0;
		UINT uiHeight = 0;
		FrameBufferAttachmentSpecification AttachmentSpec;
		UINT uiSample = 1;
		bool bSwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual FrameBufferSpecification& GetSpec() = 0;
		virtual const FrameBufferSpecification& GetSpec() const = 0;

		virtual const UINT GetColorAttachmentId(UINT uiIdx = 0) const = 0;
		virtual const UINT GetDepthStencilAttachmentId() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(UINT uiWidth, UINT uiHeight) = 0;
		virtual void ClearColorAttachment(UINT uiIdx, int nValue) = 0;

		virtual int ReadPixel(int nAttachmentIdx, int nX, int nY) = 0;
	};
}