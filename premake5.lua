include "./Vender/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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

    group "Dependencies"
        include "Vender/premake"
        include "Dazel/Vender/GLFW"
        include "Dazel/Vender/GLAD"
        include "Dazel/Vender/ImGui"
        include "Dazel/Vender/yaml-cpp"
        include "Dazel/Vender/Box2D"
    group ""

    include "Dazel"
    include "Sandbox"
    include "DazelEditor"
