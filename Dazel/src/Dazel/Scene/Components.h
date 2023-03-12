#pragma once

#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Dazel/Camera/Camera.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Dazel/Core/Timestep.h"

namespace DAZEL
{

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

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_Color(color) {}

		operator glm::vec4& () { return m_Color; }
		operator const glm::vec4& () const { return m_Color; }
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
















}