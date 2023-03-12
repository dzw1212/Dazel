#pragma once
#include "Dazel/Core/Core.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace DAZEL
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> CreateByCoord(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& tileSize, const glm::vec2& spriteRatio = glm::vec2(1.f));
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}