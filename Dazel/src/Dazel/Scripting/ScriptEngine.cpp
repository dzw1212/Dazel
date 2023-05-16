#include "DazelPCH.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "Dazel/Scene/Components.h"
#include "Dazel/Scene/Scene.h"

#include "glm/glm.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/attrdefs.h"

namespace DAZEL
{
	namespace Utils
	{
		static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				LOG_ERROR("Fail to open file {}", filepath);
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				LOG_ERROR("File {} size is 0", filepath);
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				LOG_ERROR("Mono read data from assembly {} failed", assemblyPath);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				std::cout << std::format("namespace={}, name={}", nameSpace, name) << std::endl;
			}
		}

		static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			MonoClass* monoClass = mono_class_from_name(image, namespaceName, className);

			if (monoClass == nullptr)
			{
				LOG_ERROR("Get class {}::{} from assembly failed", namespaceName, className);
				return nullptr;
			}

			return monoClass;
		}

		enum class Accessibility : uint8_t
		{
			None = 0,
			Private = (1 << 0),
			Internal = (1 << 1),
			Protected = (1 << 2),
			Public = (1 << 3),
		};

		// Gets the accessibility level of the given field
		static uint8_t GetFieldAccessibility(MonoClassField* field)
		{
			uint8_t accessibility = (uint8_t)Accessibility::None;
			uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

			switch (accessFlag)
			{
			case MONO_FIELD_ATTR_PRIVATE:
			{
				accessibility = (uint8_t)Accessibility::Private;
				break;
			}
			case MONO_FIELD_ATTR_FAM_AND_ASSEM:
			{
				accessibility |= (uint8_t)Accessibility::Protected;
				accessibility |= (uint8_t)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_ASSEMBLY:
			{
				accessibility = (uint8_t)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_FAMILY:
			{
				accessibility = (uint8_t)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_FAM_OR_ASSEM:
			{
				accessibility |= (uint8_t)Accessibility::Private;
				accessibility |= (uint8_t)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_PUBLIC:
			{
				accessibility = (uint8_t)Accessibility::Public;
				break;
			}
			}

			return accessibility;
		}

		// Gets the accessibility level of the given property
		static uint8_t GetPropertyAccessbility(MonoProperty* property)
		{
			uint8_t accessibility = (uint8_t)Accessibility::None;

			// Get a reference to the property's getter method
			MonoMethod* propertyGetter = mono_property_get_get_method(property);
			if (propertyGetter != nullptr)
			{
				// Extract the access flags from the getters flags
				uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

				switch (accessFlag)
				{
				case MONO_FIELD_ATTR_PRIVATE:
				{
					accessibility = (uint8_t)Accessibility::Private;
					break;
				}
				case MONO_FIELD_ATTR_FAM_AND_ASSEM:
				{
					accessibility |= (uint8_t)Accessibility::Protected;
					accessibility |= (uint8_t)Accessibility::Internal;
					break;
				}
				case MONO_FIELD_ATTR_ASSEMBLY:
				{
					accessibility = (uint8_t)Accessibility::Internal;
					break;
				}
				case MONO_FIELD_ATTR_FAMILY:
				{
					accessibility = (uint8_t)Accessibility::Protected;
					break;
				}
				case MONO_FIELD_ATTR_FAM_OR_ASSEM:
				{
					accessibility |= (uint8_t)Accessibility::Private;
					accessibility |= (uint8_t)Accessibility::Protected;
					break;
				}
				case MONO_FIELD_ATTR_PUBLIC:
				{
					accessibility = (uint8_t)Accessibility::Public;
					break;
				}
				}
			}

			// Get a reference to the property's setter method
			MonoMethod* propertySetter = mono_property_get_set_method(property);
			if (propertySetter != nullptr)
			{
				// Extract the access flags from the setters flags
				uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
				if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
					accessibility = (uint8_t)Accessibility::Private;
			}
			else
			{
				accessibility = (uint8_t)Accessibility::Private;
			}

			return accessibility;
		}

		static bool CheckMonoError(MonoError& error)
		{
			bool hasError = !mono_error_ok(&error);
			if (hasError)
			{
				unsigned short errorCode = mono_error_get_error_code(&error);
				const char* errorMessage = mono_error_get_message(&error);
				std::cout << "Mono Error!" << std::endl;
				std::cout << "Error Code: " << errorCode << std::endl;
				std::cout << "Error Message: " << errorMessage << std::endl;
				mono_error_cleanup(&error);
			}
			return hasError;
		}

		static std::string MonoStringToUTF8(MonoString* monoString)
		{
			if (monoString == nullptr || mono_string_length(monoString) == 0)
				return "";

			MonoError error;
			char* utf8 = mono_string_to_utf8_checked(monoString, &error);
			if (CheckMonoError(error))
				return "";
			std::string result(utf8);
			mono_free(utf8);
			return result;
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_mapFieldTypeNameToScriptFieldType.find(typeName);
			if (it == s_mapFieldTypeNameToScriptFieldType.end())
			{
				return ScriptFieldType::None;
			}

			return it->second;
		}

		const char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Entity:  return "Entity";
			}
			ASSERT(false, "Script field type unknown");
			return "<Invalid>";
		}

		ScriptFieldType ScriptFieldTypeToString(const std::string& strType)
		{
			if (strType == "Float")
				return ScriptFieldType::Float;
			else if (strType == "Double")
				return ScriptFieldType::Double;
			else if (strType == "Bool")
				return ScriptFieldType::Bool;
			else if (strType == "Char")
				return ScriptFieldType::Char;
			else if (strType == "Byte")
				return ScriptFieldType::Byte;
			else if (strType == "Short")
				return ScriptFieldType::Short;
			else if (strType == "Int")
				return ScriptFieldType::Int;
			else if (strType == "Long")
				return ScriptFieldType::Long;
			else if (strType == "UByte")
				return ScriptFieldType::UByte;
			else if (strType == "UShort")
				return ScriptFieldType::UShort;
			else if (strType == "UInt")
				return ScriptFieldType::UInt;
			else if (strType == "Vector2")
				return ScriptFieldType::Vector2;
			else if (strType == "Vector3")
				return ScriptFieldType::Vector3;
			else if (strType == "Vector4")
				return ScriptFieldType::Vector4;
			else if (strType == "Entity")
				return ScriptFieldType::Entity;
			else
			{
				ASSERT(false, "Script field type unknown");
				return ScriptFieldType::None;
			}
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* pRootDomain = nullptr;
		MonoAssembly* pCoreAssembly = nullptr;
		MonoImage* pCoreAssemblyImage = nullptr;
		std::filesystem::path CoreAssemblyFilepath;

		MonoDomain* pAppDomain = nullptr;
		MonoAssembly* pAppAssembly = nullptr;
		MonoImage* pAppAssemblyImage = nullptr;	
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass* pBaseEntityClass = nullptr;

		std::unordered_map<std::string, Ref<ScriptClass>> mapAllEntityClass;
		std::unordered_map<UUID, Ref<ScriptInstance>> mapAllEntityInstance;
		std::unordered_map<UUID, ScriptFieldMap> mapEntityScriptField;

		Scene* pCurrentScene = nullptr;
	};

	static ScriptEngineData *s_ScriptEngineData;


	void ScriptEngine::Init()
	{
		s_ScriptEngineData = new ScriptEngineData;
		InitMono();

		CORE_ASSERT(LoadAssembly("Resource/Script/Dazel-ScriptCore.dll"), "Load script assembly failed");

		ScriptGlue::RegisterInternalCallFunctions();
		ScriptGlue::RegisterComponents();

		s_ScriptEngineData->pBaseEntityClass = new ScriptClass("DAZEL", "Entity");

		CollectAllEntityClasses(s_ScriptEngineData->pAssembly);
	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();

		delete s_ScriptEngineData->pBaseEntityClass;
		delete s_ScriptEngineData;

		s_ScriptEngineData = nullptr;
	}

	bool ScriptEngine::LoadCoreAssembly(const std::filesystem::path& filepath)
	{
		//第二个参数允许传递一个配置文件的路径
		MonoDomain* appDomain = mono_domain_create_appdomain(const_cast<char*>("DAZEL_Mono_AppDomain"), nullptr);
		CORE_ASSERT(appDomain, "Create mono app domain failed");

		s_ScriptEngineData->pAppDomain = appDomain;

		//将s_AppDomain设为当前应用域，第二个参数表示是否强制
		mono_domain_set(s_ScriptEngineData->pAppDomain, true);

		s_ScriptEngineData->CoreAssemblyFilepath = filepath;
		s_ScriptEngineData->pCoreAssembly = Utils::LoadCSharpAssembly(filepath.string());
		s_ScriptEngineData->pCoreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->pCoreAssembly);

		return (s_ScriptEngineData->pCoreAssembly != nullptr) && (s_ScriptEngineData->pCoreAssemblyImage != nullptr);
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		//第二个参数允许传递一个配置文件的路径
		MonoDomain* appDomain = mono_domain_create_appdomain(const_cast<char*>("DAZEL_Mono_AppDomain"), nullptr);
		CORE_ASSERT(appDomain, "Create mono app domain failed");

		s_ScriptEngineData->pAppDomain = appDomain;

		//将s_AppDomain设为当前应用域，第二个参数表示是否强制
		mono_domain_set(s_ScriptEngineData->pAppDomain, true);

		s_ScriptEngineData->CoreAssemblyFilepath = filepath;
		s_ScriptEngineData->pCoreAssembly = Utils::LoadCSharpAssembly(filepath.string());
		s_ScriptEngineData->pCoreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->pCoreAssembly);

		return (s_ScriptEngineData->pCoreAssembly != nullptr) && (s_ScriptEngineData->pCoreAssemblyImage != nullptr);
	}

	void ScriptEngine::CollectAllEntityClasses(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		s_ScriptEngineData->mapAllEntityClass.clear();

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* namespaceName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			auto pMonoClass = mono_class_from_name(image, namespaceName, className);
			auto pEntityClass = mono_class_from_name(image, "DAZEL", "Entity");

			if (pMonoClass == pEntityClass)
				continue;

			std::string strFullName = namespaceName == "" ? className : std::format("{}.{}", namespaceName, className);

			if (!mono_class_is_subclass_of(pMonoClass, pEntityClass, false))
				continue;

			auto scriptClass = CreateRef<ScriptClass>(namespaceName, className);
			s_ScriptEngineData->mapAllEntityClass[strFullName] = scriptClass;

			int nFieldCount = mono_class_num_fields(pMonoClass);

			void* iter = nullptr;
			MonoClassField* field = nullptr;
			scriptClass->m_mapField.clear();
			while (field = mono_class_get_fields(pMonoClass, &iter))
			{
				const char* fieldName = mono_field_get_name(field);
				
				MonoType* type = mono_field_get_type(field);
				std::string typeName = mono_type_get_name(type);
				ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);

				scriptClass->m_mapField[fieldName] = { fieldType, fieldName, field };
			}
		}
	}

	bool ScriptEngine::IsEntityClassExists(const std::string& strClassName)
	{
		return s_ScriptEngineData->mapAllEntityClass.find(strClassName) != s_ScriptEngineData->mapAllEntityClass.end();
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetAllEntityClass()
	{
		return s_ScriptEngineData->mapAllEntityClass;
	}

	bool ScriptEngine::IsEntityInstanceExists(const UUID& uuid)
	{
		return s_ScriptEngineData->mapAllEntityInstance.find(uuid) != s_ScriptEngineData->mapAllEntityInstance.end();
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityInstance(const UUID& uuid)
	{
		auto it = s_ScriptEngineData->mapAllEntityInstance.find(uuid);
		if (it == s_ScriptEngineData->mapAllEntityInstance.end())
			return nullptr;

		return it->second;
	}

	void ScriptEngine::OnRuntimeStart(Scene* pScene)
	{
		s_ScriptEngineData->pCurrentScene = pScene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_ScriptEngineData->pCurrentScene = nullptr;
		s_ScriptEngineData->mapAllEntityInstance.clear();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& strClassName = entity.GetComponent<ScriptComponent>().m_strName;
		auto instance = CreateRef<ScriptInstance>(s_ScriptEngineData->mapAllEntityClass[strClassName], entity);
		s_ScriptEngineData->mapAllEntityInstance[entity.GetUUId()] = instance;
		instance->InvokeOnCreate();
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float fTimestep)
	{
		if (!IsEntityInstanceExists(entity.GetUUId()))
			return;
		auto instance = s_ScriptEngineData->mapAllEntityInstance[entity.GetUUId()];
		instance->InvokeOnUpdate(fTimestep);
	}

	Scene* ScriptEngine::GetCurrentScene()
	{
		return s_ScriptEngineData->pCurrentScene;
	}

	MonoAssembly* ScriptEngine::GetAssembly()
	{
		return s_ScriptEngineData->pAssembly;
	}

	MonoImage* ScriptEngine::GetAssemblyImage()
	{
		return s_ScriptEngineData->pAssemblyImage;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5/");

		//mono_jit_init_version可以指定版本，但一般让mono自己选择
		MonoDomain* rootDomain = mono_jit_init("DAZEL_Mono_JIT");
		CORE_ASSERT(rootDomain, "Init mono jit failed");

		s_ScriptEngineData->pRootDomain = rootDomain;
	}
	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_ScriptEngineData->pAppDomain);
		s_ScriptEngineData->pAppDomain = nullptr;

		mono_jit_cleanup(s_ScriptEngineData->pRootDomain);
		s_ScriptEngineData->pRootDomain = nullptr;

		
		s_ScriptEngineData->pAssembly = nullptr;
	}
	MonoObject* ScriptEngine::InstantiateClass(MonoClass* pMonoClass)
	{
		MonoObject* pClassInstance = mono_object_new(s_ScriptEngineData->pAppDomain, pMonoClass);
		mono_runtime_object_init(pClassInstance);
		return pClassInstance;
	}
	ScriptClass::ScriptClass(const std::string& strClassNamespace, const std::string& strClassName)
		: m_strClassNamespace(strClassNamespace), m_strClassName(strClassName)
	{
		m_pMonoClass = mono_class_from_name(s_ScriptEngineData->pAssemblyImage, m_strClassNamespace.c_str(), m_strClassName.c_str());
	}
	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_pMonoClass);
	}
	MonoMethod* ScriptClass::GetMethod(const std::string& strName, int nParamCnt)
	{
		return mono_class_get_method_from_name(m_pMonoClass, strName.c_str(), nParamCnt);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* pClassInstance, MonoMethod* pMethod, void** params)
	{
		return mono_runtime_invoke(pMethod, pClassInstance, params, nullptr);
	}
	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_pClassInstance = m_ScriptClass->Instantiate();

		m_pOnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_pOnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);
	
		m_pConstructor = s_ScriptEngineData->pBaseEntityClass->GetMethod(".ctor", 1);
		if (m_pConstructor)
		{
			auto uuid = entity.GetUUId();
			void* params[] =
			{
				&uuid,
			};
			m_ScriptClass->InvokeMethod(m_pClassInstance, m_pConstructor, params);
		}
		
	}
	void ScriptInstance::InvokeOnCreate()
	{
		if (!m_pClassInstance || !m_pOnCreateMethod)
			return;
		m_ScriptClass->InvokeMethod(m_pClassInstance, m_pOnCreateMethod, nullptr);
	}
	void ScriptInstance::InvokeOnUpdate(float fTimestep)
	{
		if (!m_pClassInstance || !m_pOnUpdateMethod)
			return;

		void* params[] =
		{
			&fTimestep
		};
		m_ScriptClass->InvokeMethod(m_pClassInstance, m_pOnUpdateMethod, params);
	}
	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->m_mapField;
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_pClassInstance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->m_mapField;
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_pClassInstance, field.ClassField, (void*)value);
		return true;
	}
}
