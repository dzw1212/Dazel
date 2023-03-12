#include "DazelPCH.h"
#include "SubTexture2D.h"

namespace DAZEL
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { texCoordMin.x, texCoordMax.y };
		m_TexCoords[1] = texCoordMax;
		m_TexCoords[2] = { texCoordMax.x, texCoordMin.y };
		m_TexCoords[3] = texCoordMin;
	}
	Ref<SubTexture2D> SubTexture2D::CreateByCoord(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& tileSize, const glm::vec2& spriteRatio)
	{
		glm::vec2 texCoordMin = { (coord.x * tileSize.x) / texture->GetWidth(), (coord.y * tileSize.y) / texture->GetHeight() };
		glm::vec2 texCoordMax = { ((coord.x + spriteRatio.x) * tileSize.x) / texture->GetWidth(), ((coord.y + spriteRatio.y) * tileSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, texCoordMin, texCoordMax);
	}
}