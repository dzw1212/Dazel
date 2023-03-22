#include "DazelPCH.h"

#include "Components.h"
#include "Entity.h"
#include "Scene.h"
#include "Dazel/Renderer/Renderer2D.h"


namespace DAZEL
{

	Scene::Scene()
	{

	}
	Scene::~Scene()
	{

	}
	void Scene::OnUpdateEditor(Timestep timeStep, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto quadView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : quadView)
		{
			auto [transform, sprite] = quadView.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}
	void Scene::OnUpdateRuntime(Timestep timeStep)
	{
		Camera* mainCamera = nullptr;
		glm::mat4 mainCameraTransform;
		auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : cameraView)
		{
			auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
			if (camera.m_bIsMainCamera)
			{
				mainCamera = &(camera.m_Camera);
				mainCameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

			auto quadView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : quadView)
			{
				auto [transform, sprite] = quadView.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}
	Entity Scene::CreateEntity(const std::string& strName)
	{
		 Entity entity(m_Registry.create(), this);
		 entity.AddComponent<TagComponent>(strName);
		 entity.AddComponent<TransformComponent>();
		 return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
	Entity Scene::GetPrimaryCameraEntity()
	{
		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView)
		{
			auto& cameraComponent = cameraView.get<CameraComponent>(entity);
			if (cameraComponent.m_bIsMainCamera)
				return Entity{ entity, this };
		}

		return {};
	}
	void Scene::OnViewportResize(UINT uiWidth, UINT uiHeight)
	{
		m_uiViewportWidth = uiWidth;
		m_uiViewportHeight = uiHeight;

		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView)
		{
			auto& cameraComponent = cameraView.get<CameraComponent>(entity);
			if (!cameraComponent.m_bFixedAspectRatio)
			{
				cameraComponent.m_Camera.SetViewportSize(uiWidth, uiHeight);
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdd(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdd<TagComponent>(Entity entity, TagComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_uiViewportWidth > 0 && m_uiViewportHeight > 0)
			component.m_Camera.SetViewportSize(m_uiViewportWidth, m_uiViewportHeight);
	}
	template<>
	void Scene::OnComponentAdd<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}
}
