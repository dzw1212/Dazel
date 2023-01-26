#pragma once

#include "Dazel/Core/Core.h"
#include <string>

namespace DAZEL
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual UINT GetWidth() const = 0;
		virtual UINT GetHeight() const = 0;

		virtual void SetData(void* data, UINT uiSize) = 0;
		virtual void Bind(const int nSlot = 0) const = 0;

		virtual UINT GetId() const = 0;

		virtual bool operator == (const Texture& otherTexture) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& strPath);
		static Ref<Texture2D> Create(UINT uiWidth, UINT uiHeight);
	};
}

