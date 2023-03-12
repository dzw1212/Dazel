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
		for (auto& element : layout.GetElements())
		{
			switch (element.eType)
			{
			case ShaderDataType::FLOAT:
			case ShaderDataType::VEC2:
			case ShaderDataType::VEC3:
			case ShaderDataType::VEC4:
			{
				glEnableVertexAttribArray(m_uiVertexBufferIdx);
				glVertexAttribPointer(m_uiVertexBufferIdx,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.eType),
					element.bNormalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.uiOffset);

				m_uiVertexBufferIdx++;
				break;
			}
			case ShaderDataType::INT:
			case ShaderDataType::INT2:
			case ShaderDataType::INT3:
			case ShaderDataType::INT4:
			case ShaderDataType::BOOL:
			{
				glEnableVertexAttribArray(m_uiVertexBufferIdx);
				glVertexAttribIPointer(m_uiVertexBufferIdx,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.eType),
					layout.GetStride(),
					(const void*)element.uiOffset);

				m_uiVertexBufferIdx++;
				break;
			}
			case ShaderDataType::MAT3:
			case ShaderDataType::MAT4:
			{
				UINT uiCount = element.GetComponentCount();
				for (UINT i = 0; i < uiCount; ++i)
				{
					glEnableVertexAttribArray(m_uiVertexBufferIdx);
					glVertexAttribPointer(m_uiVertexBufferIdx,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.eType),
						element.bNormalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.uiOffset);
					glVertexAttribDivisor(m_uiVertexBufferIdx, 1);
					m_uiVertexBufferIdx++;
				}
				break;
			}

			default:
				CORE_ASSERT(false, "Unknown Shader Data Type");
				break;
			}
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