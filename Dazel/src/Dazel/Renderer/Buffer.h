#pragma once

#include "Dazel/Core/Core.h"
#include "glad/glad.h"

namespace DAZEL
{
	enum class ShaderDataType
	{
		NONE = 0,
		FLOAT,
		INT,
		UINT,
		BOOL,
		VEC2,
		VEC3,
		VEC4,
		INT2,
		INT3,
		INT4,
		UINT2,
		UINT3,
		UINT4,
		MAT3,
		MAT4,
	};

	static UINT GetShaderDataSize(ShaderDataType eType)
	{
		switch (eType)
		{
			case ShaderDataType::FLOAT:		return sizeof(float);
			case ShaderDataType::INT:		return sizeof(int);
			case ShaderDataType::UINT:		return sizeof(unsigned int);
			case ShaderDataType::BOOL:		return sizeof(bool);
			case ShaderDataType::VEC2:		return sizeof(float) * 2;
			case ShaderDataType::VEC3:		return sizeof(float) * 3;
			case ShaderDataType::VEC4:		return sizeof(float) * 4;
			case ShaderDataType::INT2:		return sizeof(int) * 2;
			case ShaderDataType::INT3:		return sizeof(int) * 3;
			case ShaderDataType::INT4:		return sizeof(int) * 4;
			case ShaderDataType::UINT2:		return sizeof(unsigned int) * 2;
			case ShaderDataType::UINT3:		return sizeof(unsigned int) * 3;
			case ShaderDataType::UINT4:		return sizeof(unsigned int) * 4;
			case ShaderDataType::MAT3:		return sizeof(float) * 3 * 3;
			case ShaderDataType::MAT4:		return sizeof(float) * 4 * 4;
		}
		return 0;
	}

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType eType)
	{
		switch (eType)
		{
			case ShaderDataType::FLOAT:
			case ShaderDataType::VEC2:
			case ShaderDataType::VEC3:
			case ShaderDataType::VEC4:
			case ShaderDataType::MAT3:
			case ShaderDataType::MAT4:
				return GL_FLOAT;
			case ShaderDataType::INT:
			case ShaderDataType::INT2:
			case ShaderDataType::INT3:
			case ShaderDataType::INT4:
				return GL_INT;
			case ShaderDataType::UINT:
			case ShaderDataType::UINT2:
			case ShaderDataType::UINT3:
			case ShaderDataType::UINT4:
				return GL_UNSIGNED_INT;
			case ShaderDataType::BOOL:
				return GL_BOOL;
		}
		return GL_FLOAT;
	}

	struct BufferElement
	{
		ShaderDataType eType;
		std::string strName;
		UINT uiSize;
		UINT uiOffset;
		bool bNormalized;

		BufferElement() {};
		BufferElement(ShaderDataType eType, const std::string& strName, bool bNormalized = false)
			: eType(eType), strName(strName), uiSize(GetShaderDataSize(eType)), uiOffset(0), bNormalized(bNormalized)
		{

		}

		const UINT GetComponentCount() const
		{
			switch (eType)
			{
			case ShaderDataType::FLOAT:
			case ShaderDataType::INT:
			case ShaderDataType::UINT:
			case ShaderDataType::BOOL:
				return 1;
			case ShaderDataType::VEC2:
			case ShaderDataType::INT2:
			case ShaderDataType::UINT2:
				return 2;
			case ShaderDataType::VEC3:
			case ShaderDataType::INT3:
			case ShaderDataType::UINT3:
				return 3;
			case ShaderDataType::VEC4:
			case ShaderDataType::INT4:
			case ShaderDataType::UINT4:
				return 4;
			case ShaderDataType::MAT3:
				return 3 * 3;
			case ShaderDataType::MAT4:
				return 4 * 4;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_vecElements(elements) 
		{
			UINT uiOffset = 0;
			for (auto& element : m_vecElements)
			{
				element.uiOffset = uiOffset;
				uiOffset += element.uiSize;
			}
			m_uiStride = uiOffset;
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_vecElements; }
		inline const UINT GetStride() const { return m_uiStride; }
	private:
		std::vector<BufferElement> m_vecElements;
		UINT m_uiStride; //一组数据的长度
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() = 0;

		virtual void SetData(const void* data, UINT uiSize) = 0;

		static Ref<VertexBuffer> Create(float* pfVertices, UINT uiSize);
		static Ref<VertexBuffer> Create(UINT uiSize);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual UINT GetCount() const = 0;

		static Ref<IndexBuffer> Create(UINT* puiIndices, UINT uiCount);
		static Ref<IndexBuffer> Create(UINT uiCount);
	};
}