#pragma once

#include "Dazel/Renderer/Shader.h"
#include "glad/glad.h"

#include <unordered_map>

namespace DAZEL
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& strFilePath);
		OpenGLShader(const std::string& strName, const std::string& strVertexFile, const std::string& strFragmentFile);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& strName, const glm::mat4& mat4) override;
		virtual void SetFloat4(const std::string& strName, const glm::vec4& vec4) override;
		virtual void SetFloat3(const std::string& strName, const glm::vec3& vec3) override;
		virtual void SetInt(const std::string& strName, int nVal) override;
		virtual void SetIntArray(const std::string& strName, int* pnVal, UINT uiCount) override;

		virtual const std::string& GetName() const override;

		void UploadUniformMat4(const std::string& strName, const glm::mat4& mat4);
		void UploadUniformMat3(const std::string& strName, const glm::mat3& mat3);
		void UploadUniformfVec1(const std::string& strName, const glm::vec1& fvec1);
		void UploadUniformfVec2(const std::string& strName, const glm::vec2& fvec2);
		void UploadUniformfVec3(const std::string& strName, const glm::vec3& fvec3);
		void UploadUniformfVec4(const std::string& strName, const glm::vec4& fvec4);
		void UploadUniformIVec1(const std::string& strName, const glm::ivec1& ivec1);
		void UploadUniformIVec2(const std::string& strName, const glm::ivec2& ivec2);
		void UploadUniformIVec3(const std::string& strName, const glm::ivec3& ivec3);
		void UploadUniformIVec4(const std::string& strName, const glm::ivec4& ivec4);
		void UploadUniformUIVec1(const std::string& strName, const glm::uvec1& uvec1);
		void UploadUniformUIVec2(const std::string& strName, const glm::uvec2& uvec2);
		void UploadUniformUIVec3(const std::string& strName, const glm::uvec3& uvec3);
		void UploadUniformUIVec4(const std::string& strName, const glm::uvec4& uvec4);

		void UploadUniformIntArray(const std::string& strName, int* pnVal, UINT uiCount);

	private:
		std::unordered_map<GLenum, std::string> SplitShaderSource(const std::string& strShaderSrc);
		void CompileShader(const std::unordered_map<GLenum, std::string>& mapShaderSrc);

		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& mapShaderSrc);
		void CompileOrGetOpenGLBinaries();

		void CreateProgram();
		void Reflect(GLenum eStage, const std::vector<UINT>& vecShaderData);

	private:
		UINT m_uiRendererProgramId = 0;
		std::string m_strName;

		std::string m_strFilePath;
		std::unordered_map<GLenum, std::vector<UINT>> m_mapVulkanSPIRV;
		std::unordered_map<GLenum, std::vector<UINT>> m_mapOpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_mapOpenGLSrcCode;
	};
}
