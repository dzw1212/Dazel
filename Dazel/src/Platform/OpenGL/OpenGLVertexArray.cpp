#include "DazelPCH.h"
#include "OpenGLVertexArray.h"

namespace DAZEL
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_uiId);
		glBindVertexArray(m_uiId);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_uiId);
	}
	void OpenGLVertexArray::Bind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(m_uiId);
	}
	void OpenGLVertexArray::Unbind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		PROFILE_FUNCTION();

		glBindVertexArray(m_uiId);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		CORE_ASSERT(layout.GetElements().size() > 0, "VertexBuffer need set layout first");
		for (BYTE idx = 0; idx < layout.GetElements().size(); ++idx)
		{
			auto& element = layout.GetElements()[idx];
			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.eType),
				element.bNormalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.uiOffset);
		}

		m_vecVertexBuffer.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		PROFILE_FUNCTION();

		glBindVertexArray(m_uiId);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}