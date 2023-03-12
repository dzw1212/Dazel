#pragma once

#include "Dazel/Renderer/VertexArray.h"

namespace DAZEL
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vecVertexBuffer; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		UINT m_uiId;
		UINT m_uiVertexBufferIdx = 0;
		std::vector<Ref<VertexBuffer>> m_vecVertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}