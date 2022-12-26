outputdir = "%{cfg.longname}_%{cfg.architecture}"

IncludeDir = {
    ["GLFW"] = "Dazel/Vender/GLFW/include",
    ["GLAD"] = "Dazel/Vender/GLAD/include",
    ["ImGui"] = "Dazel/Vender/ImGui"
}

startproject "Sandbox"

workspace "Dazel"
	architecture "x64"

	configurations {"Debug", "Release", "Publish"}

    include "Dazel/Vender/GLFW"
    include "Dazel/Vender/GLAD"
    include "Dazel/Vender/ImGui"

	project "Dazel"
		location "Dazel"
		kind "SharedLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

        pchheader "DazelPCH.h"
        pchsource "Dazel/src/DazelPCH.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}

		includedirs
		{
            "%{prj.name}/src",
			"%{prj.name}/Vender/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
		}

        links
        {
            "GLFW",
            "GLAD",
            "ImGui",
            "opengl32.lib",
        }

		filter "system:windows"
			cppdialect "C++latest"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"DAZEL_PLATFORM_WINDOWS",
				"DAZEL_BUILD_DLL",
                "DAZEL_ENABLE_ASSERTS",
                "GLFW_INCLUDE_NONE",
			}

			postbuildcommands
			{
				("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox/")
			}

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
            buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
            buildoptions "/MD"
			optimize "On"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
            buildoptions "/MD"
			optimize "On"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}

		includedirs
		{
            "Dazel/src",
			"Dazel/Vender/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
		}

		links "Dazel"

		filter "system:windows"
			cppdialect "C++latest"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"DAZEL_PLATFORM_WINDOWS",
                "DAZEL_ENABLE_ASSERTS",
                "GLFW_INCLUDE_NONE",
			}

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
            buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
            buildoptions "/MD"
			optimize "On"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
            buildoptions "/MD"
			optimize "On"
