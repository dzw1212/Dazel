#include "DazelPCH.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace DAZEL
{
	GLenum ShaderTypeFromString(const std::string& strType)
	{
		if (strType == "vertex")
			return GL_VERTEX_SHADER;
		else if (strType == "fragment")
			return GL_FRAGMENT_SHADER;

		CORE_ASSERT(false, "Unsupport shader type");
		return GL_FALSE;
	}
	std::string ShaderTypeToString(GLenum eType)
	{
		if (eType == GL_VERTEX_SHADER)
			return "vertex";
		else if (eType == GL_FRAGMENT_SHADER)
			return "fragment";

		CORE_ASSERT(false, "Unsupport shader type");
		return "";
	}
	OpenGLShader::OpenGLShader(const std::string& strFilePath)
	{
		PROFILE_FUNCTION();

		const std::string strShaderSrc = ReadFile(strFilePath);
		auto mapShaderSrc = SplitShaderSource(strShaderSrc);
		CompileShader(mapShaderSrc);

		m_strName = GetShaderNameByPath(strFilePath);
	}
	OpenGLShader::OpenGLShader(const std::string& strName, const std::string& strVertexFile, const std::string& strFragmentFile)
		: m_strName(strName)
	{
		PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> mapShaderSrc;
		mapShaderSrc[GL_VERTEX_SHADER] = ReadShaderFromFile(strVertexFile);
		mapShaderSrc[GL_FRAGMENT_SHADER] = ReadShaderFromFile(strFragmentFile);
		CompileShader(mapShaderSrc);
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
			CORE_ASSERT(ShaderTypeFromString(type), "Unsupport shader source type");

			size_t nextLinePos = strShaderSrc.find_first_not_of("\r\n", eol);
			pos = strShaderSrc.find(typeToken, nextLinePos);
			if (pos == std::string::npos)
				mapShaderSrc[ShaderTypeFromString(type)] = strShaderSrc.substr(nextLinePos, strShaderSrc.size() - 1);
			else
				mapShaderSrc[ShaderTypeFromString(type)] = strShaderSrc.substr(nextLinePos, pos - nextLinePos);
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
				CORE_ASSERT(false, "Compile {} Shader Failed, Info is {}", ShaderTypeToString(shaderType), infoLog.data());

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
			CORE_ASSERT(false, "Link Shader Failed, Info is {}", infoLog.data());
		}

		// Always detach shaders after a successful link.
		for (auto shaderId : vecShaderId)
			glDetachShader(program, shaderId);

		m_uiRendererProgramId = program;
	}
	
}