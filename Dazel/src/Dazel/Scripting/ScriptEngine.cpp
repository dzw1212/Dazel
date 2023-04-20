#include "DazelPCH.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace DAZEL
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

			std::cout << "---row: " << i << std::endl;
			std::cout << "---namespace: " << nameSpace << std::endl;
			std::cout << "---name: " <<name << std::endl;
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

	struct ScriptEngineData
	{
		MonoDomain* pRootDomain = nullptr;
		MonoDomain* pAppDomain = nullptr;

		MonoAssembly* pScriptAssembly = nullptr;
	};

	static ScriptEngineData *s_ScriptEngineData;


	void ScriptEngine::Init()
	{
		s_ScriptEngineData = new ScriptEngineData;
		InitMono();
	}
	void ScriptEngine::Shutdown()
	{
		delete s_ScriptEngineData;
	}
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5/");

		//mono_jit_init_version可以指定版本，但一般让mono自己选择
		MonoDomain* rootDomain = mono_jit_init("DAZEL_Mono_JIT");
		CORE_ASSERT(rootDomain, "Init mono jit failed");

		s_ScriptEngineData->pRootDomain = rootDomain;

		//第二个参数允许传递一个配置文件的路径
		MonoDomain* appDomain = mono_domain_create_appdomain(const_cast<char*>("DAZEL_Mono_AppDomain"), nullptr);
		CORE_ASSERT(appDomain, "Create mono app domain failed");

		s_ScriptEngineData->pAppDomain = appDomain;

		//将s_AppDomain设为当前应用域，第二个参数表示是否强制
		mono_domain_set(s_ScriptEngineData->pAppDomain, true);

		s_ScriptEngineData->pScriptAssembly = LoadCSharpAssembly("Resource/Script/Dazel-ScriptCore.dll");
		PrintAssemblyTypes(s_ScriptEngineData->pScriptAssembly);
	}
}
