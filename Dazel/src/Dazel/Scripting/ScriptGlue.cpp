#include "DazelPCH.h"
#include "ScriptGlue.h"
#include "glm/glm.hpp"

#include "mono/metadata/object.h"

namespace DAZEL
{

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("DAZEL.InternalCalls::"#Name, Name)


	static void TransformComponent_GetPosition(UUID entityUUId, glm::vec3* outPos)
	{

	}

	static void TransformComponent_SetPosition(UUID entityUUId, const glm::vec3& pos)
	{

	}

	void ScriptGlue::RegisterInternalCallFunctions()
	{
		ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		ADD_INTERNAL_CALL(TransformComponent_SetPosition);
	}
}
