#pragma once

#include "entt.hpp"
#include "Scene.h"
#include "Components.h"

#include <utility>

namespace DAZEL
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityId, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&& ... args)
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity already has this component");
			T& component = m_Scene->GetRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdd<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		void AddOrReplaceComponent(Args&& ... args)
		{
			if (HasComponent<T>())
			{
				m_Scene->GetRegistry().emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			}
			else
			{
				T& component = m_Scene->GetRegistry().emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
				m_Scene->OnComponentAdd<T>(*this, component);
			}
		}

		template<typename T, typename... Args>
		T& UpdateComponent(Args&& ... args)
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
			return m_Scene->GetRegistry().replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
			m_Scene->GetRegistry().remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->GetRegistry().all_of<T>(m_EntityHandle);
		}

		template<typename... Types>
		bool AllofComponent()
		{
			return m_Scene->GetRegistry().all_of<Types...>(m_EntityHandle);
		}

		template<typename... Types>
		bool AnyofComponent()
		{
			return m_Scene->GetRegistry().any_of<Types...>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
			return m_Scene->GetRegistry().get<T>(m_EntityHandle);
		}

		UINT GetId() const { return (UINT)m_EntityHandle; }

		UINT64 GetUUId();
		void SetUUId(UINT64& uuid);

		operator bool() const { return m_EntityHandle != entt::null; }
		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle
				&& m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const
		{
			return m_EntityHandle != other.m_EntityHandle
				|| m_Scene != other.m_Scene;
		}
		operator entt::entity() const { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene{ nullptr };
	};
}
