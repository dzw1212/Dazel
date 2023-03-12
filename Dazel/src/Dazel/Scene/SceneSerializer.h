#pragma once

#include "Scene.h"
#include "Dazel/Core/Core.h"

namespace DAZEL
{
	class SceneSerializer
	{
	public:
		SceneSerializer() = default;
		SceneSerializer(const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);

		bool Serialize(const std::string& strFilepath);
		bool SerializeBinary(const std::string& strFilepath);
		
		bool Deserialize(const std::string& strFilepath);
		bool DeserializeBinary(const std::string& strFilepath);

	private:
		Ref<Scene> m_Scene;
	};
}