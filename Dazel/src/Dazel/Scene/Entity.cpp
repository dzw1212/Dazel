#include "DazelPCH.h"

#include "Entity.h"
#include "Components.h"

namespace DAZEL
{
	Entity::Entity(entt::entity entityId, Scene* scene)
		: m_EntityHandle(entityId), m_Scene(scene)
	{
	}
	UINT64 Entity::GetUUId()
	{
		return GetComponent<IDComponent>().m_UUId;

	}
	void Entity::SetUUId(UINT64& uuid)
	{
		GetComponent<IDComponent>().m_UUId = uuid;
	}
}
