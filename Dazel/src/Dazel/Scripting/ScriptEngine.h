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
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace DAZEL
{
	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity,
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};

	static std::unordered_map<std::string, ScriptFieldType> s_mapFieldTypeNameToScriptFieldType =
	{
		{ "System.Single",	ScriptFieldType::Float },
		{ "System.Double",	ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char",	ScriptFieldType::Char },
		{ "System.Int16",	ScriptFieldType::Short },
		{ "System.Int32",	ScriptFieldType::Int },
		{ "System.Int64",	ScriptFieldType::Long },
		{ "System.Byte",	ScriptFieldType::Byte },
		{ "System.UInt16",	ScriptFieldType::UShort },
		{ "System.UInt32",	ScriptFieldType::UInt },
		{ "System.UInt64",	ScriptFieldType::ULong },

		{ "DAZEL.Vector2",	ScriptFieldType::Vector2 },
		{ "DAZEL.Vector3",	ScriptFieldType::Vector3 },
		{ "DAZEL.Vector4",	ScriptFieldType::Vector4 },

		{ "DAZEL.Entity",	ScriptFieldType::Entity },
	};



	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& strClassNamespace, const std::string& strClassName);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& strName, int nParamCnt);
		MonoObject* InvokeMethod(MonoObject* pClassInstance, MonoMethod* pMethod, void** params = nullptr);

		std::map<std::string, ScriptField> m_mapField;

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

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			SetFieldValueInternal(name, &value);
		}

	private:
		inline static char s_FieldValueBuffer[8];
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);

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
		static MonoAssembly* GetAssembly();
		static MonoImage* GetAssemblyImage();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* pMonoClass);

		friend class ScriptClass;
	};
}
