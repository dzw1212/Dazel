#pragma once

#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Dazel/Camera/Camera.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Dazel/Renderer/Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Dazel/Core/UUID.h"
#include "Dazel/Core/Core.h"
#include "Dazel/Core/Timestep.h"

namespace DAZEL
{
	enum class BodyType
	{
		STATIC,
		DYNAMIC,
		KINEMATIC,
		MAX,
	};

	struct IDComponent
	{
		UUID m_UUId;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& uuid) : m_UUId(uuid) {}
		IDComponent(UINT64 u64Id) : m_UUId(u64Id) {}

		operator UUID& () { return m_UUId; }
		operator const UUID& () const { return m_UUId; }
	};

	struct TagComponent
	{
		std::string m_strTag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& strTag) : m_strTag(strTag) {}

		operator std::string& () { return m_strTag; }
		operator const std::string& () const { return m_strTag; }
	};

	struct TransformComponent
	{
		glm::vec3 m_Position = glm::vec3{ 0.f };
		glm::vec3 m_Rotation = glm::vec3{ 0.f };
		glm::vec3 m_Scale = glm::vec3{ 1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.f), m_Position);
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
			glm::mat4 scale = glm::scale(glm::mat4(1.f), m_Scale);

			return translation * rotation * scale;
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 m_Color{ 1.f };
		Ref<Texture2D> m_Texture;
		int m_nTileFactor = 1;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_Color(color) {}

		operator glm::vec4& () { return m_Color; }
		operator const glm::vec4& () const { return m_Color; }
	};

	struct CircleRendererComponent
	{
		glm::vec4 m_Color{ 1.f };
		float m_fThickness = 0.5f;
		float m_fFade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera m_Camera;
		bool m_bIsMainCamera = false;
		bool m_bFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const SceneCamera& camera) : m_Camera(camera) {}
		CameraComponent(const glm::mat4& proj) : m_Camera(proj) {}

		operator SceneCamera& () { return m_Camera; }
		operator const SceneCamera& () const { return m_Camera; }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* m_Instance = nullptr;

		bool IsInitiated() { return m_Instance != nullptr; }

		std::function<void()> InstanceNewFunc;
		std::function<void()> InstanceDeleteFunc;

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timeStep) {}

		template<typename T>
		void Bind()
		{
			InstanceNewFunc = [&]() { m_Instance = new T(); };
			InstanceDeleteFunc = [&]() { delete (T*)m_Instance; m_Instance = nullptr; };
		}
	};

	struct RigidBody2DComponent
	{
		BodyType m_Type = BodyType::STATIC;
		bool m_bFixedRotation = false;

		void* m_RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

	};

	struct BoxCollider2DComponent
	{
		glm::vec2 m_Offset = { 0.f, 0.f };
		glm::vec2 m_Size = { 0.5f, 0.5f };
		float m_fDensity = 1.f;
		float m_fFriction = 0.5f;
		float m_fRestitution = 0.f;
		float m_fRestitutionThreshold = 0.5f;


		void* m_RuntimeFixture = nullptr;


		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 m_Offset = { 0.f, 0.f };
		float m_fRadius = 0.5f;
		float m_fDensity = 1.f;
		float m_fFriction = 0.5f;
		float m_fRestitution = 0.f;
		float m_fRestitutionThreshold = 0.5f;

		void* m_RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<
			TransformComponent, 
			SpriteRendererComponent,
			CircleRendererComponent, 
			CameraComponent, 
			NativeScriptComponent,
			RigidBody2DComponent, 
			BoxCollider2DComponent, 
			CircleCollider2DComponent
		>;














}
