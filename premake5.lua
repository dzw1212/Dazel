outputdir = "%{cfg.longname}_%{cfg.architecture}"

IncludeDir = {
    ["GLFW"] = "Dazel/Vender/GLFW/include",
    ["GLAD"] = "Dazel/Vender/GLAD/include",
    ["ImGui"] = "Dazel/Vender/ImGui",
    ["glm"] = "Dazel/Vender/glm",
    ["stb_image"] = "Dazel/Vender/stb_image",
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
		kind "StaticLib"
		language "C++"
        staticruntime "on"
        cppdialect "C++latest"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

        pchheader "DazelPCH.h"
        pchsource "Dazel/src/DazelPCH.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
            "%{prj.name}/Vender/glm/glm/**.hpp",
            "%{prj.name}/Vender/glm/glm/**.inl",
            "%{prj.name}/Vender/stb_image/**.h",
            "%{prj.name}/Vender/stb_image/**.cpp",
		}

		includedirs
		{
            "%{prj.name}/src",
			"%{prj.name}/Vender/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
		}

        links
        {
            "GLFW",
            "GLAD",
            "ImGui",
            "opengl32.lib",
        }

		filter "system:windows"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"DAZEL_PLATFORM_WINDOWS",
				"DAZEL_BUILD_DLL",
                "DAZEL_ENABLE_ASSERTS",
                "GLFW_INCLUDE_NONE",
                "_CRT_SECURE_NOT_WARNINGS",
			}

			-- postbuildcommands
			-- {
			-- 	("{COPYFILE} %{cfg.buildtarget.relpath} \"../bin/"..outputdir.."/Sandbox/\"")
			-- }

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
            runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
            runtime "Release"
			optimize "on"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
            runtime "Release"
			optimize "on"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
        cppdialect "C++latest"
        staticruntime "on"

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
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
		}

		links "Dazel"

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"DAZEL_PLATFORM_WINDOWS",
                "DAZEL_ENABLE_ASSERTS",
                "_CRT_SECURE_NOT_WARNINGS",
			}

		filter "configurations:Debug"
			defines "DAZEL_DEBUG"
            runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "DAZEL_RELEASE"
            runtime "Release"
			optimize "on"

		filter "configurations:Publish"
			defines "DAZEL_PUBLISH"
            runtime "Release"
			optimize "on"
