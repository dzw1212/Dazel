#pragma once

#include <filesystem>
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace DAZEL
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static bool LoadAssembly(const std::filesystem::path& filepath);

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* pMonoClass);

		friend class ScriptClass;
	};

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
}
