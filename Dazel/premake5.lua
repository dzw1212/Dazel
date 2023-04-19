project "Dazel"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
    cppdialect "C++latest"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "DazelPCH.h"
    pchsource "src/DazelPCH.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "Vender/glm/glm/**.hpp",
        "Vender/glm/glm/**.inl",
        "Vender/stb_image/**.h",
        "Vender/stb_image/**.cpp",
        "Vender/entt/**.hpp",
        "Vender/ImGuizmo/ImGuizmo.h",
        "Vender/ImGuizmo/ImGuizmo.cpp",
    }

    defines
    {
        "DAZEL_PLATFORM_WINDOWS",
        "DAZEL_BUILD_DLL",
        "DAZEL_ENABLE_ASSERTS",
        "_CRT_SECURE_NOT_WARNINGS",
        "GLFW_INCLUDE_NONE",
    }

    includedirs
    {
        "src",
        "Vender/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.mono}",
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "Box2D",
        "opengl32.lib",
        "%{Library.mono}",
    }

    filter "files:Vender/ImGuizmo/**.cpp"
    flags {"NoPCH"}

    filter "system:windows"
        systemversion "latest"

        links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

    filter "configurations:Debug"
        defines "DAZEL_DEBUG"
        runtime "Debug"
        symbols "on"

        links
        {
            "%{Library.ShaderC_Debug}",
            "%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
        }

    filter "configurations:Release"
        defines "DAZEL_RELEASE"
        runtime "Release"
        optimize "on"

        links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

    filter "configurations:Publish"
        defines "DAZEL_PUBLISH"
        runtime "Release"
        optimize "on"

        links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}