#pragma once

#include <string>
#include "glm/glm.hpp"

namespace DAZEL
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetMat4(const std::string& strName, const glm::mat4& mat4) = 0;
		virtual void SetFloat4(const std::string& strName, const glm::vec4& vec4) = 0;
		virtual void SetFloat3(const std::string& strName, const glm::vec3& vec3) = 0;
		virtual void SetInt(const std::string& strName, int nVal) = 0;

		std::string ReadShaderFromFile(const std::string& strFilePath);
		std::string ReadFile(const std::string& strFilePath);
		std::string GetShaderNameByPath(const std::string& strFilePath);

		static Ref<Shader> Create(const std::string& strName, const std::string& strVertexFile, const std::string& strFragmentFile);
		static Ref<Shader> Create(const std::string& strFilePath);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		bool Exist(const std::string& strName);
		Ref<Shader> Get(const std::string& strName);
		Ref<Shader> Load(const std::string& strFilePath);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_mapShader;
	};
}