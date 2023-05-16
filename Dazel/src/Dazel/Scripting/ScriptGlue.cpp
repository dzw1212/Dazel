#include "DazelPCH.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "glm/glm.hpp"

#include "Dazel/Scene/Components.h"
#include "box2d/b2_body.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "Dazel/Core/KeyCodes.h"
#include "Dazel/Core/Input.h"

namespace DAZEL
{

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	template<typename... ComponentType>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(ComponentType).name();
				std::cout << typeName << std::endl;
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = std::format("DAZEL.{}", structName);

				std::cout << structName << std::endl;
				std::cout << managedTypename << std::endl;

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetAssemblyImage());
				if (!managedType)
				{
					LOG_ERROR("Script componentType {} not find", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<ComponentType>(); };
			}(), ...);
	}

	template<typename... ComponentType>
	static void RegisterComponent(ComponentGroup<ComponentType...>)
	{
		RegisterComponent<ComponentType...>();
	}


	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetCurrentScene();
		if (!scene)
			return false;
		Entity entity = scene->GetEntityByUUID(entityID);
		if (!entity)
			return false;
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		if (s_EntityHasComponentFuncs.find(managedType) == s_EntityHasComponentFuncs.end())
			return false;
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("DAZEL.InternalCall::"#Name, Name)

	static void TransformComponent_GetPosition(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetCurrentScene();
		if (!scene)
			return;
		Entity entity = scene->GetEntityByUUID(entityID);
		if (!entity)
			return;

		*outTranslation = entity.GetComponent<TransformComponent>().m_Position;
	}

	static void TransformComponent_SetPosition(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetCurrentScene();
		if (!scene)
			return;
		Entity entity = scene->GetEntityByUUID(entityID);
		if (!entity)
			return;

		entity.GetComponent<TransformComponent>().m_Position = *translation;
	}

	static bool Input_IsKeyDown(int nKeyCode)
	{
		return Input::IsKeyPressed(nKeyCode);
	}

	static void RigidBody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetCurrentScene();
		if (!scene)
			return;
		Entity entity = scene->GetEntityByUUID(entityID);
		if (!entity)
			return;

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetCurrentScene();
		if (!scene)
			return;
		Entity entity = scene->GetEntityByUUID(entityID);
		if (!entity)
			return;

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.m_RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	void ScriptGlue::RegisterInternalCallFunctions()
	{
		ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		ADD_INTERNAL_CALL(TransformComponent_SetPosition);
		ADD_INTERNAL_CALL(Input_IsKeyDown);
		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
	}
}
