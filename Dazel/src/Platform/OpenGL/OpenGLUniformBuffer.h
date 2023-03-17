#pragma once
#include "Dazel/Renderer/UniformBuffer.h"

namespace DAZEL
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(UINT uiSize, UINT uiBinding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* pData, UINT uiSize, UINT uiOffset = 0) override;
	private:
		UINT m_uiId;
	};
}