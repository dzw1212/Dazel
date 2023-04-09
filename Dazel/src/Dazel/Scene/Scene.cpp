#include "DazelPCH.h"

#include "Components.h"
#include "Entity.h"
#include "Scene.h"
#include "Dazel/Renderer/Renderer2D.h"
#include "Dazel/Core/UUID.h"
#include "Dazel/Core/Core.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"


namespace DAZEL
{

	static b2BodyType BodyTypeToB2BodyType(BodyType eType)
	{
		switch (eType)
		{
		case BodyType::STATIC:
			return b2BodyType::b2_staticBody;
		case BodyType::DYNAMIC:
			return b2BodyType::b2_dynamicBody;
		case BodyType::KINEMATIC:
			return b2BodyType::b2_kinematicBody;
		default:
			CORE_ASSERT(false, "Unsupport Body Type");
			break;
		}

		return b2BodyType::b2_staticBody;
	}

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
			if (sprite.m_Texture)
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.m_Texture, sprite.m_nTileFactor, sprite.m_Color, (int)entity);
			else
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.m_Color, (int)entity);

			auto circleView = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : circleView)
			{
				auto [transform, circle] = circleView.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.m_Color, circle.m_fThickness, circle.m_fFade, (int)entity);
				Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(0.0, 1.0, 0.0, 1.0));
			}
		}

		Renderer2D::DrawLine(glm::vec3(-10.f, 0.f, 0.f), glm::vec3(10.f, 0.f, 0.f), glm::vec4(1.f));
		Renderer2D::DrawLine(glm::vec3(0.f, -10.f, 0.f), glm::vec3(0.f, 10.f, 0.f), glm::vec4(1.f));

		Renderer2D::EndScene();
	}
	void Scene::OnUpdateRuntime(Timestep timeStep)
	{
		//Physical
		const int nVelocityIterations = 6;
		const int nPositionIterations = 2;
		m_PhysicalWorld->Step(timeStep.GetSeconds(), nVelocityIterations, nPositionIterations);
		
		auto rigidBodyView = m_Registry.view<RigidBody2DComponent>();
		for (auto entity : rigidBodyView)
		{
			Entity rigidBodyEntity = { entity, this };
			b2Body* body = (b2Body*)rigidBodyEntity.GetComponent<RigidBody2DComponent>().m_RuntimeBody;

			auto& transformComponent = rigidBodyEntity.GetComponent<TransformComponent>();
			transformComponent.m_Position.x = body->GetPosition().x;
			transformComponent.m_Position.y = body->GetPosition().y;
			transformComponent.m_Rotation.z = body->GetAngle();
		}

		//Render 2D
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
				if (sprite.m_Texture)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.m_Texture, sprite.m_nTileFactor, sprite.m_Color, (int)entity);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.m_Color, (int)entity);
			}

			auto circleView = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : circleView)
			{
				auto [transform, circle] = circleView.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.m_Color, circle.m_fThickness, circle.m_fFade, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}
	Entity Scene::CreateEntity(const std::string& strName)
	{
		return CreateEntityWithUUID(UUID(), strName);
	}
	Entity Scene::CreateEntityWithUUID(UINT64 uuid, const std::string& strName)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<IDComponent>(uuid);
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

	template<typename ComponentType>
	static void CopyComponent(const Ref<Scene>& dup, const Ref<Scene>& origin, const std::unordered_map<UUID, UINT>& mapDupEntities)
	{
		auto componentView = origin->GetRegistry().view<ComponentType>();
		for (auto entity : componentView)
		{
			auto originEntity = Entity(entity, origin.get());
			UUID uuid = originEntity.GetComponent<IDComponent>().m_UUId;
			auto iter = mapDupEntities.find(uuid);
			if (iter == mapDupEntities.end())
				continue;
			Entity dupEntity = Entity(static_cast<entt::entity>(iter->second), dup.get());
			dupEntity.AddOrReplaceComponent<ComponentType>(originEntity.GetComponent<ComponentType>());
		}
	}

	template<typename ComponentType>
	static void CopyComponentIfExists(Entity dup, Entity origin)
	{
		if (origin.HasComponent<ComponentType>())
		{
			dup.AddOrReplaceComponent<ComponentType>(origin.GetComponent<ComponentType>());
		}
	}

	Ref<Scene> Scene::Copy(const Ref<Scene>& origin)
	{
		Ref<Scene> dupScene = CreateRef<Scene>();

		dupScene->m_uiViewportWidth = origin->m_uiViewportWidth;
		dupScene->m_uiViewportHeight = origin->m_uiViewportHeight;

		std::unordered_map<UUID, UINT> mapDupEntities;

		auto idView = origin->m_Registry.view<IDComponent>();
		for (auto entity : idView)
		{
			auto originEntity = Entity(entity, origin.get());
			auto newEntity = dupScene->CreateEntityWithUUID(originEntity.GetUUId());
			UUID uuid = newEntity.GetComponent<IDComponent>().m_UUId;
			UINT entityId = newEntity.GetId();
			mapDupEntities[uuid] = entityId;
		}

		CopyComponent<TransformComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<SpriteRendererComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<CircleRendererComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<CameraComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<NativeScriptComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<RigidBody2DComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<BoxCollider2DComponent>(dupScene, origin, mapDupEntities);
		CopyComponent<CircleCollider2DComponent>(dupScene, origin, mapDupEntities);

		return dupScene;
	}

	void Scene::CopyEntity(Entity origin)
	{
		std::string name = origin.GetComponent<TagComponent>().m_strTag;
		auto newEntity = CreateEntity(name + "_" + std::to_string(origin.GetId()));

		CopyComponentIfExists<TransformComponent>(newEntity, origin);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, origin);
		CopyComponentIfExists<CircleRendererComponent>(newEntity, origin);
		CopyComponentIfExists<CameraComponent>(newEntity, origin);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, origin);
		CopyComponentIfExists<RigidBody2DComponent>(newEntity, origin);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, origin);
		CopyComponentIfExists<CircleCollider2DComponent>(newEntity, origin);
	}

	void Scene::OnRuntimeStart()
	{
		b2Vec2 gravity(0.0f, -10.0f); //定义重力矢量
		m_PhysicalWorld = new b2World(gravity);

		auto rigidBodyView = m_Registry.view<RigidBody2DComponent>();
		for (auto entity : rigidBodyView)
		{
			Entity rigidBodyEntity = { entity, this };
			auto& transformComponent = rigidBodyEntity.GetComponent<TransformComponent>();
			auto& rigidBodyComponent = rigidBodyEntity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = BodyTypeToB2BodyType(rigidBodyComponent.m_Type);
			bodyDef.fixedRotation = rigidBodyComponent.m_bFixedRotation;
			bodyDef.position.Set(transformComponent.m_Position.x, transformComponent.m_Position.y);
			bodyDef.angle = transformComponent.m_Rotation.z;
			b2Body* body = m_PhysicalWorld->CreateBody(&bodyDef);
			rigidBodyComponent.m_RuntimeBody = body;

			if (rigidBodyEntity.HasComponent<BoxCollider2DComponent>())
			{
				auto& boxColliderComponent = rigidBodyEntity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(transformComponent.m_Scale.x * boxColliderComponent.m_Size.x, transformComponent.m_Scale.y * boxColliderComponent.m_Size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxColliderComponent.m_fDensity;
				fixtureDef.friction = boxColliderComponent.m_fFriction;
				fixtureDef.restitution = boxColliderComponent.m_fRestitution;
				fixtureDef.restitutionThreshold = boxColliderComponent.m_fRestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				boxColliderComponent.m_RuntimeFixture = fixture;
			}

			if (rigidBodyEntity.HasComponent<CircleCollider2DComponent>())
			{
				auto& circleColliderComponent = rigidBodyEntity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(circleColliderComponent.m_Offset.x, circleColliderComponent.m_Offset.y);
				circleShape.m_radius = circleColliderComponent.m_fRadius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circleColliderComponent.m_fDensity;
				fixtureDef.friction = circleColliderComponent.m_fFriction;
				fixtureDef.restitution = circleColliderComponent.m_fRestitution;
				fixtureDef.restitutionThreshold = circleColliderComponent.m_fRestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				circleColliderComponent.m_RuntimeFixture = fixture;
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicalWorld;
		m_PhysicalWorld = nullptr;
	}

	template<typename T>
	void Scene::OnComponentAdd(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdd<IDComponent>(Entity entity, IDComponent& component)
	{
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
	template<>
	void Scene::OnComponentAdd<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdd<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
}
