#include "DazelPCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace DAZEL
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* pfVertices, UINT uiSize)
	{
		PROFILE_FUNCTION();

		glCreateBuffers(1, &m_uiId);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
		glBufferData(GL_ARRAY_BUFFER, uiSize, pfVertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::OpenGLVertexBuffer(UINT uiSize)
	{
		PROFILE_FUNCTION();

		glCreateBuffers(1, &m_uiId);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
		glBufferData(GL_ARRAY_BUFFER, uiSize, nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_uiId);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, UINT uiSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, uiSize, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(UINT* puiIndices, UINT uiCount)
		: m_uiCount(uiCount)
	{
		PROFILE_FUNCTION();

		glCreateBuffers(1, &m_uiId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(unsigned int), puiIndices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(UINT uiCount)
	{
		PROFILE_FUNCTION();

		glCreateBuffers(1, &m_uiId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_uiId);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}