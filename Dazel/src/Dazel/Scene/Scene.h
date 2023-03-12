#pragma once

#include "Dazel/Core/Timestep.h"
#include "Dazel/Camera/EditorCamera.h"

#include "entt.hpp"

namespace DAZEL
{
	class Entity;

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
	private:
		entt::registry m_Registry;
		UINT m_uiViewportWidth;
		UINT m_uiViewportHeight;
	};
}