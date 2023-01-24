#include "DazelPCH.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace DAZEL
{
	std::string Shader::ReadShaderFromFile(const std::string& strFilePath)
	{
		std::string strShader;
		std::ifstream fileStream(strFilePath, std::ios::in);
		std::string strLine = "";
		while (!fileStream.eof())
		{
			getline(fileStream, strLine);
			strShader.append(strLine + "\n");
		}
		fileStream.close();
		return strShader;
	}

	std::string Shader::ReadFile(const std::string& strFilePath)
	{
		std::string content;
		std::ifstream in(strFilePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			content.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&content[0], content.size());
			in.close();
		}
		return content;
	}

	std::string Shader::GetShaderNameByPath(const std::string& strFilePath)
	{
		// 分隔符"/"和"\\"不能混用
		size_t begin = strFilePath.find_last_of("/");
		if (begin == std::string::npos)
		{
			begin = strFilePath.find_last_of("\\");
			if (begin == std::string::npos)
				begin = 0;
			else
				begin += 1;
		}
		else
			begin += 1;
		size_t end = strFilePath.find(".", begin);
		if (end == std::string::npos)
			end = strFilePath.size() - 1;
		return strFilePath.substr(begin, end - begin);
	}

	Ref<Shader> Shader::Create(const std::string& strName, const std::string& strVertexFile, const std::string& strFragmentFile)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(strName, strVertexFile, strFragmentFile);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& strFilePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(strFilePath);
		}
		}
		CORE_ASSERT(false, "RendererAPI {0} is currently not support", (int)Renderer::GetAPI());
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		if (m_mapShader.find(shader->GetName()) != m_mapShader.end())
		{
			LOG_WARN("Add duplicate shader : {}", shader->GetName());
			return;
		}
		m_mapShader[shader->GetName()] = shader;
	}

	bool ShaderLibrary::Exist(const std::string& strName)
	{
		return m_mapShader.find(strName) != m_mapShader.end();
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& strName)
	{
		auto it = m_mapShader.find(strName);
		CORE_ASSERT(it != m_mapShader.end(), "Not find shader by name : {}", strName);
		return it->second;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& strFilePath)
	{
		auto shader = Shader::Create(strFilePath);
		Add(shader);
		return shader;
	}

}