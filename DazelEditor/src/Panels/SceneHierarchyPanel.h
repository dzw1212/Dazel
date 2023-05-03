#pragma once

#include "Dazel/Core/Core.h"
#include "Dazel/Scene/Scene.h"
#include "Dazel/Scene/Entity.h"

namespace DAZEL
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context)
			: m_Context(context), m_SelectedEntity({})
		{}

		void SetContext(const Ref<Scene>& context);
		
		void OnImGuiRender();

		void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }
		Entity GetSelectedEntity() const { return m_SelectedEntity; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template<typename ComponentType>
		void AddPopupComponent(const std::string& strShowName);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};
}
