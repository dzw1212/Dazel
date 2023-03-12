#include "DazelPCH.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include "glm/glm.hpp"

#include "yaml-cpp/yaml.h"

namespace YAML 
{
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace DAZEL
{
    YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    static bool SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "EntityUId";
        out << YAML::Value << entity.GetId();
        out << YAML::Key << "Components";
        out << YAML::BeginMap;

        std::string strEntityName;

        if (entity.HasComponent<TagComponent>())
        {
            auto& tagComponent = entity.GetComponent<TagComponent>();
            out << YAML::Key << "TagComponent";
            out << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "Name";
            out << YAML::Value << tagComponent.m_strTag;
            out << YAML::EndMap;

            strEntityName = tagComponent.m_strTag;
        }

        LOG_INFO("Deserialize entity Id:{}, name:{}", entity.GetId(), strEntityName);

        if (entity.HasComponent<TransformComponent>())
        {
            auto& transformComponent = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "TransformComponent";
            out << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "Position";
            out << YAML::Value << transformComponent.m_Position;
            out << YAML::Key << "Rotation";
            out << YAML::Value << transformComponent.m_Rotation;
            out << YAML::Key << "Scale";
            out << YAML::Value << transformComponent.m_Scale;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<CameraComponent>())
        {
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.m_Camera;
            out << YAML::Key << "CameraComponent";
            out << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "Main Camera";
            out << YAML::Value << cameraComponent.m_bIsMainCamera;
            out << YAML::Key << "Fixed AspectRatio";
            out << YAML::Value << cameraComponent.m_bFixedAspectRatio;
            out << YAML::Key << "Camera";

            out << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "Projection Type";
            out << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "AspectRatio";
            out << YAML::Value << camera.GetAspectRatio();

            out << YAML::Key << "Orthographic Size";
            out << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "Orthographic Near Clip";
            out << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "Orthographic Far Clip";
            out << YAML::Value << camera.GetOrthographicFarClip();

            out << YAML::Key << "Perspective Vertical FOV";
            out << YAML::Value << camera.GetPerspectiveVerticalFOV();
            out << YAML::Key << "Perspective Near Clip";
            out << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "Perspective Far Clip";
            out << YAML::Value << camera.GetPerspectiveFarClip();

            out << YAML::EndMap;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "Color";
            out << YAML::Value << spriteRendererComponent.m_Color;
            out << YAML::EndMap;
        }

        out << YAML::EndMap;
        out << YAML::EndMap;
        return out.good();
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
       : m_Scene(scene)
    {
    }

    void SceneSerializer::SetScene(const Ref<Scene>& scene)
    {
        m_Scene = scene;
    }

    bool SceneSerializer::Serialize(const std::string& strFilepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << "Untitled";
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        m_Scene->GetRegistry().each([&](auto entityId)
            {
                Entity entity = { entityId, m_Scene.get() };
                if (!entity)
                    return;
                SerializeEntity(out, entity);
            });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(strFilepath);
        fout << out.c_str();

        return out.good();
    }

    bool SceneSerializer::SerializeBinary(const std::string& strFilepath)
    {
        return false;
    }

    bool SceneSerializer::Deserialize(const std::string& strFilepath)
    {
        std::ifstream in(strFilepath);
        std::stringstream stringStream;
        stringStream << in.rdbuf();

        YAML::Node root = YAML::Load(stringStream.str());
        if (!root["Scene"])
            return false;

        std::string strSceneName = root["Scene"].as<std::string>();
        LOG_INFO("Load Scene[{}] from {}", strSceneName, strFilepath);

        auto entities = root["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                auto deserializeEntity = m_Scene->CreateEntity();

                UINT uid = entity["EntityUId"].as<UINT>();
                auto componentsNode = entity["Components"];
                
                auto tagComponentNode = componentsNode["TagComponent"];
                auto& tagComponent = deserializeEntity.GetComponent<TagComponent>();
                tagComponent.m_strTag = tagComponentNode["Name"].as<std::string>();

                auto transformComponentNode = componentsNode["TransformComponent"];
                auto& transformComponent = deserializeEntity.GetComponent<TransformComponent>();

                transformComponent.m_Position = transformComponentNode["Position"].as<glm::vec3>();
                transformComponent.m_Rotation = transformComponentNode["Rotation"].as<glm::vec3>();
                transformComponent.m_Scale = transformComponentNode["Scale"].as<glm::vec3>();


                auto cameraComponentNode = componentsNode["CameraComponent"];
                if (cameraComponentNode)
                {
                    auto& cameraComponent = deserializeEntity.AddComponent<CameraComponent>();
                    cameraComponent.m_bIsMainCamera = cameraComponentNode["Main Camera"].as<bool>();
                    cameraComponent.m_bFixedAspectRatio = cameraComponentNode["Fixed AspectRatio"].as<bool>();
                    auto cameraNode = cameraComponentNode["Camera"];
                    cameraComponent.m_Camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["Projection Type"].as<int>());
                    cameraComponent.m_Camera.SetAspectRatio(cameraNode["AspectRatio"].as<float>());
                    cameraComponent.m_Camera.SetOrthographicSize(cameraNode["Orthographic Size"].as<float>());
                    cameraComponent.m_Camera.SetOrthographicNearClip(cameraNode["Orthographic Near Clip"].as<float>());
                    cameraComponent.m_Camera.SetOrthographicFarClip(cameraNode["Orthographic Far Clip"].as<float>());
                    cameraComponent.m_Camera.SetPerspectiveVerticalFOV(cameraNode["Perspective Vertical FOV"].as<float>());
                    cameraComponent.m_Camera.SetPerspectiveNearClip(cameraNode["Perspective Near Clip"].as<float>());
                    cameraComponent.m_Camera.SetPerspectiveFarClip(cameraNode["Perspective Far Clip"].as<float>());
                }

                auto spriteRendererComponentNode = componentsNode["SpriteRendererComponent"];
                if (spriteRendererComponentNode)
                {
                    deserializeEntity.AddComponent<SpriteRendererComponent>(spriteRendererComponentNode["Color"].as<glm::vec4>());
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeBinary(const std::string& strFilepath)
    {
        return false;
    }
}


