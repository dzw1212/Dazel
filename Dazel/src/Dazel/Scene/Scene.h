#pragma once

#include "entt.hpp"

#include <unordered_map>

class b2World;

namespace DAZEL
{
	class Entity;
	class Timestep;
	class EditorCamera;

	class Scene
	{
	public:
		Scene();
		~Scene();


		Entity CreateEntity(const std::string& strName = std::string());
		Entity CreateEntityWithUUID(UINT64 uuid, const std::string& strName = std::string());
		void DestroyEntity(Entity entity);

		Entity GetEntityByUUID(UINT64 uuid);

		Entity GetPrimaryCameraEntity();

		void OnUpdateEditor(Timestep timeStep, EditorCamera& camera);
		void OnUpdateRuntime(Timestep timeStep, bool bTickPhysical = true);
		void OnUpdateSimulation(Timestep timeStep, EditorCamera& camera);
		void OnViewportResize(UINT uiWidth, UINT uiHeight);

		entt::registry& GetRegistry() { return m_Registry; }

		static Ref<Scene> Copy(const Ref<Scene>& origin);

		void CopyEntity(Entity origin);

	public:
		template<typename T>
		void OnComponentAdd(Entity entity, T& component);

		template<typename... ComponentTypes>
		auto GetAllEntities()
		{
			return m_Registry.view<ComponentTypes...>();
		}
	public:
		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnPhysical2DStart();
		void OnPhysical2DStop();
	public:
		void RenderScene(EditorCamera& camera);
		void TickPhysical(Timestep timeStep);
	private:
		entt::registry m_Registry;
		UINT m_uiViewportWidth;
		UINT m_uiViewportHeight;

		b2World* m_PhysicalWorld = nullptr;

		std::unordered_map<UINT64, entt::entity> m_mapAllEntity;
	};
}
