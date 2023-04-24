#include "DazelPCH.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"

namespace DAZEL
{

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("DAZEL.InternalCalls::"#Name, Name)


	static void CppFunction()
	{
		std::cout << "call in C++" << std::endl;
	}

	void ScriptGlue::RegisterInternalCallFunctions()
	{
		ADD_INTERNAL_CALL(CppFunction);
	}
}
