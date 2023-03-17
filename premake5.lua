include "./Vender/premake/premake_customization/solution_items.lua"

outputdir = "%{cfg.longname}_%{cfg.architecture}"

workspace "Dazel"
	architecture "x64"
    startproject "DazelEditor"

	configurations {"Debug", "Release", "Publish"}

    solution_items {".editorconfig"}

    flags {"MultiProcessorCompile"}

    defines 
    {
        "DAZEL_PLATFORM_WINDOWS",
    }

    IncludeDir = {
        ["GLFW"] = "%{wks.location}/Dazel/Vender/GLFW/include",
        ["GLAD"] = "%{wks.location}/Dazel/Vender/GLAD/include",
        ["ImGui"] = "%{wks.location}/Dazel/Vender/ImGui",
        ["glm"] = "%{wks.location}/Dazel/Vender/glm",
        ["stb_image"] = "%{wks.location}/Dazel/Vender/stb_image",
        ["entt"] = "%{wks.location}/Dazel/Vender/entt/include",
        ["yaml_cpp"] = "%{wks.location}/Dazel/Vender/yaml-cpp/include",
        ["ImGuizmo"] = "%{wks.location}/Dazel/Vender/ImGuizmo",
    }

    group "Dependencies"
        include "Vender/premake"
        include "Dazel/Vender/GLFW"
        include "Dazel/Vender/GLAD"
        include "Dazel/Vender/ImGui"
        include "Dazel/Vender/yaml-cpp"
    group ""

    include "Dazel"
    include "Sandbox"
    include "DazelEditor"
