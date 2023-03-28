#pragma once

#include "entt.hpp"

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
		void DestroyEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		void OnUpdateEditor(Timestep timeStep, EditorCamera& camera);
		void OnUpdateRuntime(Timestep timeStep);
		void OnViewportResize(UINT uiWidth, UINT uiHeight);

		entt::registry& GetRegistry() { return m_Registry; }
	public:
		template<typename T>
		void OnComponentAdd(Entity entity, T& component);
	public:
		void OnRuntimePlay();
		void OnRuntimeStop();
	private:
		entt::registry m_Registry;
		UINT m_uiViewportWidth;
		UINT m_uiViewportHeight;

		b2World* m_PhysicalWorld;
	};
}
