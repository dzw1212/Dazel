#pragma once
#include "Dazel/Core/Core.h"

namespace DAZEL
{
	class UniformBuffer
	{
	public:
		UniformBuffer() = default;
		virtual ~UniformBuffer() {}

		virtual void SetData(const void* pData, UINT uiSize, UINT uiOffset = 0) = 0;
	
		static Ref<UniformBuffer> Create(UINT uiSize, UINT uiBinding);
	private:
		
	};
}