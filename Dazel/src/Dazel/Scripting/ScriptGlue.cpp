#include "DazelPCH.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "glm/glm.hpp"

#include "mono/metadata/object.h"

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
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Hazel.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					HZ_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
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

	void ScriptGlue::RegisterInternalCallFunctions()
	{
		ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		ADD_INTERNAL_CALL(TransformComponent_SetPosition);
		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
