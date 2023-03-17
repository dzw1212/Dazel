#include "DazelPCH.h"
#include "OpenGLUniformBuffer.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace DAZEL
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(UINT uiSize, UINT uiBinding)
	{
		glCreateBuffers(1, &m_uiId);
		glBindBufferBase(GL_UNIFORM_BUFFER, uiBinding, m_uiId);
		glNamedBufferData(m_uiId, uiSize, nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_uiId);
	}
	void OpenGLUniformBuffer::SetData(const void* pData, UINT uiSize, UINT uiOffset)
	{
		glNamedBufferSubData(m_uiId, uiOffset, uiSize, pData);
	}
}