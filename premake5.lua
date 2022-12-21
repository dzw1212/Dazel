outputdir = "%{cfg.longname}_%{cfg.architecture}"

IncludeDir = {
    ["GLFW"] = "Dazel/Vender/GLFW/include",
}

startproject "Sandbox"

workspace "Dazel"
	architecture "x64"

	configurations {"Debug", "Release", "Publish"}

    include "Dazel/Vender/GLFW"

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
		}

        links
        {
            "GLFW",
            "opengl32.lib"
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
			}

			postbuildcommands
			{
				("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox/")
			}

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
			optimize "On"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
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
			}

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
			optimize "On"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
			optimize "On"
