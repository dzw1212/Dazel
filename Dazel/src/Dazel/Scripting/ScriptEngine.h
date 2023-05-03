#pragma once

#include "Dazel/Scene/Entity.h"

#include <filesystem>
#include <string>
#include <unordered_map>


extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace DAZEL
{
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& strClassNamespace, const std::string& strClassName);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& strName, int nParamCnt);
		MonoObject* InvokeMethod(MonoObject* pClassInstance, MonoMethod* pMethod, void** params = nullptr);

	private:
		std::string m_strClassNamespace;
		std::string m_strClassName;

		MonoClass* m_pMonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float fTimestep);

	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_pClassInstance = nullptr;

		MonoMethod* m_pOnCreateMethod = nullptr;
		MonoMethod* m_pOnUpdateMethod = nullptr;

		MonoMethod* m_pConstructor = nullptr;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static bool LoadAssembly(const std::filesystem::path& filepath);

		static void CollectAllEntityClasses(MonoAssembly* assembly);
		static bool IsEntityClassExists(const std::string& strClassName);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetAllEntityClass();

		static bool IsEntityInstanceExists(const UUID& uuid);


		static void OnRuntimeStart(Scene* pScene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float fTimestep);

		static Scene* GetCurrentScene();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* pMonoClass);

		friend class ScriptClass;
	};
}
