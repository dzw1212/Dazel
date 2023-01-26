#pragma once

#include "Dazel/Renderer/Buffer.h"

namespace DAZEL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* pfVertices, UINT uiSize);
		OpenGLVertexBuffer(UINT uiSize);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
		virtual const BufferLayout& GetLayout() override { return m_Layout; };

		virtual void SetData(const void* data, UINT uiSize) override;

	private:
		UINT m_uiId;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(UINT* puiIndices, UINT uiCount);
		OpenGLIndexBuffer(UINT uiCount);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual UINT GetCount() const override { return m_uiCount; }
	private:
		UINT m_uiId;
		UINT m_uiCount;
	};
}