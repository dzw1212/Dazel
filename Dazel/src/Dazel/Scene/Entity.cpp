#include "DazelPCH.h"

#include "Entity.h"
#include "Components.h"

namespace DAZEL
{
	Entity::Entity(entt::entity entityId, Scene* scene)
		: m_EntityHandle(entityId), m_Scene(scene)
	{
	}
}