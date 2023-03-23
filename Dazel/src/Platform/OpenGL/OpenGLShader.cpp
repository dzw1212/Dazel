#include "DazelPCH.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <filesystem>

#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"
#include "spirv_cross/spirv_glsl.hpp"

#include "Dazel/Core/Timer.h"


namespace DAZEL
{
	namespace Utils
	{
		struct GLStageConvertion
		{
			const char* name;
			const char* keyword;
			const char* OpenGLCacheExtension;
			const char* VulkanCacheExtension;
			shaderc_shader_kind shadercKind;
		};

		std::map<GLenum, GLStageConvertion> g_mapGLStageConvertDetail = {
			{GL_VERTEX_SHADER, 
				{"GL_VERTEX_SHADER", 
				"vertex", 
				".cached_opengl.vert", 
				".cached_vulkan.vert", 
				shaderc_glsl_vertex_shader}
			},
			{GL_FRAGMENT_SHADER,
				{"GL_FRAGMENT_SHADER",
				"fragment",
				".cached_opengl.frag",
				".cached_vulkan.frag",
				shaderc_glsl_fragment_shader}
			},
		};

		static GLenum ShaderTypeFromString(const std::string& strType)
		{
			if (strType == "vertex")
				return GL_VERTEX_SHADER;
			else if (strType == "fragment")
				return GL_FRAGMENT_SHADER;

			CORE_ASSERT(false, "Unsupport shader type");
			return GL_FALSE;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum eStage)
		{
			auto it = g_mapGLStageConvertDetail.find(eStage);
			if (it != g_mapGLStageConvertDetail.end())
				return it->second.shadercKind;

			CORE_ASSERT(false, "Unsupport shader stage");
			return (shaderc_shader_kind)0;
		}

		//固定长度字符串用const char*，否则用std::string
		static const char* GLShaderStageToString(GLenum eStage)
		{
			auto it = g_mapGLStageConvertDetail.find(eStage);
			if (it != g_mapGLStageConvertDetail.end())
				return it->second.name;

			CORE_ASSERT(false, "Unsupport shader stage");
			return nullptr;
		}

		static const char* ShaderTypeToString(GLenum eType)
		{
			auto it = g_mapGLStageConvertDetail.find(eType);
			if (it != g_mapGLStageConvertDetail.end())
				return it->second.keyword;

			CORE_ASSERT(false, "Unsupport shader type");
			return nullptr;
		}

		static const char* GLShaderStageToOpenGLCachedFileExtension(GLenum eStage)
		{
			auto it = g_mapGLStageConvertDetail.find(eStage);
			if (it != g_mapGLStageConvertDetail.end())
				return it->second.OpenGLCacheExtension;

			CORE_ASSERT(false, "Unsupport shader type");
			return nullptr;
		}

		static const char* GLShaderStageToVulkanCachedFileExtension(GLenum eStage)
		{
			auto it = g_mapGLStageConvertDetail.find(eStage);
			if (it != g_mapGLStageConvertDetail.end())
				return it->second.VulkanCacheExtension;

			CORE_ASSERT(false, "Unsupport shader type");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			return "assert/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string strCacheDir = GetCacheDirectory();
			if (!std::filesystem::exists(strCacheDir))
			{
				std::filesystem::create_directories(strCacheDir);
			}
		}
	}
	
	
	OpenGLShader::OpenGLShader(const std::string& strFilePath)
		: m_strFilePath(strFilePath)
	{
		PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		const std::string strShaderSrc = ReadFile(strFilePath);
		auto mapShaderSrc = SplitShaderSource(strShaderSrc);

		//CompileShader(mapShaderSrc);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(mapShaderSrc);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			CORE_LOG_TRACE("create shader took {}ms", timer.ElapsedMillis());
		}

		m_strName = GetShaderNameByPath(strFilePath);
	}
	OpenGLShader::OpenGLShader(const std::string& strName, const std::string& strVertexFile, const std::string& strFragmentFile)
		: m_strName(strName)
	{
		PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> mapShaderSrc;
		mapShaderSrc[GL_VERTEX_SHADER] = ReadShaderFromFile(strVertexFile);
		mapShaderSrc[GL_FRAGMENT_SHADER] = ReadShaderFromFile(strFragmentFile);
		
		CompileOrGetVulkanBinaries(mapShaderSrc);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}
	OpenGLShader::~OpenGLShader()
	{
		PROFILE_FUNCTION();

		glDeleteProgram(m_uiRendererProgramId);
		m_uiRendererProgramId = 0;
	}
	void OpenGLShader::Bind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(m_uiRendererProgramId);
	}
	void OpenGLShader::Unbind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetMat4(const std::string& strName, const glm::mat4& mat4)
	{
		UploadUniformMat4(strName, mat4);
	}

	void OpenGLShader::SetFloat4(const std::string& strName, const glm::vec4& vec4)
	{
		UploadUniformfVec4(strName, vec4);
	}

	void OpenGLShader::SetFloat3(const std::string& strName, const glm::vec3& vec3)
	{
		UploadUniformfVec3(strName, vec3);
	}

	void OpenGLShader::SetInt(const std::string& strName, int nVal)
	{
		UploadUniformIVec1(strName, glm::ivec1(nVal));
	}

	void OpenGLShader::SetIntArray(const std::string& strName, int* pnVal, UINT uiCount)
	{
		UploadUniformIntArray(strName, pnVal, uiCount);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_strName;
	}
	void OpenGLShader::UploadUniformMat4(const std::string& strName, const glm::mat4& mat4)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& strName, const glm::mat3& mat3)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat3));
	}
	void OpenGLShader::UploadUniformfVec1(const std::string& strName, const glm::vec1& fvec1)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform1f(location, fvec1.x);
	}
	void OpenGLShader::UploadUniformfVec2(const std::string& strName, const glm::vec2& fvec2)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform2f(location, fvec2.x, fvec2.y);
	}
	void OpenGLShader::UploadUniformfVec3(const std::string& strName, const glm::vec3& fvec3)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform3f(location, fvec3.x, fvec3.y, fvec3.z);
	}
	void OpenGLShader::UploadUniformfVec4(const std::string& strName, const glm::vec4& fvec4)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform4f(location, fvec4.x, fvec4.y, fvec4.z, fvec4.w);
	}
	void OpenGLShader::UploadUniformIVec1(const std::string& strName, const glm::ivec1& ivec1)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform1i(location, ivec1.x);
	}
	void OpenGLShader::UploadUniformIVec2(const std::string& strName, const glm::ivec2& ivec2)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform2i(location, ivec2.x, ivec2.y);
	}
	void OpenGLShader::UploadUniformIVec3(const std::string& strName, const glm::ivec3& ivec3)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform3i(location, ivec3.x, ivec3.y, ivec3.z);
	}
	void OpenGLShader::UploadUniformIVec4(const std::string& strName, const glm::ivec4& ivec4)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform4i(location, ivec4.x, ivec4.y, ivec4.z, ivec4.w);
	}
	void OpenGLShader::UploadUniformUIVec1(const std::string& strName, const glm::uvec1& uvec1)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform1ui(location, uvec1.x);
	}
	void OpenGLShader::UploadUniformUIVec2(const std::string& strName, const glm::uvec2& uvec2)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform2ui(location, uvec2.x, uvec2.y);
	}
	void OpenGLShader::UploadUniformUIVec3(const std::string& strName, const glm::uvec3& uvec3)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform3ui(location, uvec3.x, uvec3.y, uvec3.z);
	}
	void OpenGLShader::UploadUniformUIVec4(const std::string& strName, const glm::uvec4& uvec4)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform4ui(location, uvec4.x, uvec4.y, uvec4.z, uvec4.w);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& strName, int* pnVal, UINT uiCount)
	{
		auto location = glGetUniformLocation(m_uiRendererProgramId, strName.c_str());
		glUniform1iv(location, uiCount, pnVal);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::SplitShaderSource(const std::string& strShaderSrc)
	{
		PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> mapShaderSrc;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = strShaderSrc.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = strShaderSrc.find_first_of("\r\n", pos);
			CORE_ASSERT(eol != std::string::npos, "Shader source syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = strShaderSrc.substr(begin, eol - begin);
			CORE_ASSERT(Utils::ShaderTypeFromString(type), "Unsupport shader source type");

			size_t nextLinePos = strShaderSrc.find_first_not_of("\r\n", eol);
			pos = strShaderSrc.find(typeToken, nextLinePos);
			if (pos == std::string::npos)
				mapShaderSrc[Utils::ShaderTypeFromString(type)] = strShaderSrc.substr(nextLinePos, strShaderSrc.size() - 1);
			else
				mapShaderSrc[Utils::ShaderTypeFromString(type)] = strShaderSrc.substr(nextLinePos, pos - nextLinePos);
		}

		return mapShaderSrc;
	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& mapShaderSrc)
	{
		PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		std::vector<GLuint> vecShaderId;

		for (auto& iter : mapShaderSrc)
		{
			GLenum shaderType = iter.first;
			const std::string& strShader = iter.second;

			// Create an empty shader handle
			GLuint shaderId = glCreateShader(shaderType);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)strShader.c_str();
			glShaderSource(shaderId, 1, &source, 0);

			// Compile the shader
			glCompileShader(shaderId);

			GLint isCompiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderId);

				// Use the infoLog as you see fit.
				CORE_ASSERT(false, std::format("Compile {} Shader Failed, Info is {}", Utils::ShaderTypeToString(shaderType), infoLog.data()));

				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shaderId);

			vecShaderId.push_back(shaderId);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto shaderId : vecShaderId)
				glDeleteShader(shaderId);

			// Use the infoLog as you see fit.
			CORE_ASSERT(false, std::format("Link Shader Failed, Info is {}", infoLog.data()));
		}

		// Always detach shaders after a successful link.
		for (auto shaderId : vecShaderId)
			glDetachShader(program, shaderId);

		m_uiRendererProgramId = program;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& mapShaderSrc)
	{
		GLuint program = glCreateProgram();

		//创建shaderc编译器
		shaderc::Compiler compiler;

		//创建并设置shaderc编译选项
		shaderc::CompileOptions compileOpts;
		compileOpts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		compileOpts.SetSourceLanguage(shaderc_source_language_glsl);
		const bool bOptimize = true;
		if (bOptimize)
			compileOpts.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDir = Utils::GetCacheDirectory();

		auto& shaderData = m_mapVulkanSPIRV;
		shaderData.clear();
		for (auto&& [eStage, strSource] : mapShaderSrc)
		{
			std::filesystem::path shaderFilePath = m_strFilePath;
			std::filesystem::path cachedPath = cacheDir /
				(shaderFilePath.filename().string() + Utils::GLShaderStageToVulkanCachedFileExtension(eStage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[eStage];
				data.resize(size / sizeof(UINT));
				in.read((char*)data.data(), size);

				CORE_LOG_INFO("Read Vulkan Binary, file:{}, size:{}", cachedPath, size);
			}
			else
			{
				//将GLSL shader编译为SPV文件
				shaderc::SpvCompilationResult res = compiler.CompileGlslToSpv(strSource,
					Utils::GLShaderStageToShaderC(eStage), m_strFilePath.c_str(), compileOpts);
				
				CORE_LOG_INFO("Compile Vulkan Shader, file:{}", m_strFilePath);

				if (res.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					CORE_ASSERT(false, std::format("Compile Vulkan Shader failed, error msg:{}", res.GetErrorMessage()));
				}

				shaderData[eStage] = std::vector<UINT>(res.begin(), res.end());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[eStage];
					out.write((char*)data.data(), data.size() * sizeof(UINT));
					out.flush();
					out.close();

					CORE_LOG_INFO("Save Vulkan Compile Result in:{}", cachedPath);
				}
			}
		}

		for (auto&& [eStage, data] : shaderData)
		{
			Reflect(eStage, data);
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_mapOpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions compileOpts;
		compileOpts.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		compileOpts.SetSourceLanguage(shaderc_source_language_glsl);
		const bool bOptimize = true;
		if (bOptimize)
			compileOpts.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDir = Utils::GetCacheDirectory();

		shaderData.clear();
		for (auto&& [eStage, spirv] : m_mapVulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_strFilePath;
			std::filesystem::path cachedPath = cacheDir /
				(shaderFilePath.filename().string() + Utils::GLShaderStageToOpenGLCachedFileExtension(eStage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[eStage];
				data.resize(size / sizeof(UINT));
				in.read((char*)data.data(), size);

				CORE_LOG_INFO("read OpenGL binary, file:{}, size:{}", cachedPath, size);
			}
			else
			{
				//使用SPIRV_Cross将Vulkan SPV转为OpenGL SourceCode
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_mapOpenGLSrcCode[eStage] = glslCompiler.compile();
				auto& source = m_mapOpenGLSrcCode[eStage];

				CORE_LOG_INFO("Convert Vulkan SPV to OpenGL source");

				shaderc::SpvCompilationResult res = compiler.CompileGlslToSpv(source,
					Utils::GLShaderStageToShaderC(eStage), m_strFilePath.c_str(), compileOpts);

				if (res.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					CORE_ASSERT(false, std::format("Compile OpenGL Shader failed, error msg:{}", res.GetErrorMessage()));
				}

				shaderData[eStage] = std::vector<UINT>(res.begin(), res.end());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[eStage];
					out.write((char*)data.data(), data.size() * sizeof(UINT));
					out.flush();
					out.close();

					CORE_LOG_INFO("Save Vulkan Compile Result in:{}", cachedPath);
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> vecShaderId;
		for (auto&& [eStage, spirv] : m_mapOpenGLSPIRV)
		{
			GLuint uiShaderId = vecShaderId.emplace_back(glCreateShader(eStage));
			glShaderBinary(1, &uiShaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(UINT));
			glSpecializeShader(uiShaderId, "main", 0, nullptr, nullptr); //根据需求自动生成高度优化的shader代码
			glAttachShader(program, uiShaderId);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto shaderId : vecShaderId)
				glDeleteShader(shaderId);

			// Use the infoLog as you see fit.
			CORE_ASSERT(false, std::format("Link Shader Failed, Info is {}", infoLog.data()));
		}

		// Always detach shaders after a successful link.
		for (auto shaderId : vecShaderId)
		{
			glDetachShader(program, shaderId);
			glDeleteShader(shaderId);
		}

		m_uiRendererProgramId = program;
	}

	void OpenGLShader::Reflect(GLenum eStage, const std::vector<UINT>& vecShaderData)
	{
		spirv_cross::Compiler compiler(vecShaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		CORE_LOG_INFO("OpenGLShader::Reflect stage:{}, file:{}", Utils::GLShaderStageToString(eStage), m_strFilePath);
		CORE_LOG_INFO("Resource UniformBuffer size:{}", resources.uniform_buffers.size());
		CORE_LOG_INFO("Resource SampledImage size:{}", resources.sampled_images.size());

		CORE_LOG_INFO("UniformBuffer:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& type = compiler.get_type(resource.base_type_id);
			auto bufferSize = compiler.get_declared_struct_size(type);
			auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding); //获取修饰符binding
			auto memberCount = type.member_types.size();

			CORE_LOG_INFO("name:{}", resource.name);
			CORE_LOG_INFO("size:{}", bufferSize);
			CORE_LOG_INFO("binding:{}", binding);
			CORE_LOG_INFO("member count:{}", memberCount);
		}
	}
	
}
