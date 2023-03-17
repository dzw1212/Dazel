project "Dazel"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++latest"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin_int/" .. outputdir .. "/%{prj.name}")

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
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib",
    }

    filter "files:Vender/ImGuizmo/**.cpp"
    flags {"NoPCH"}

    filter "system:windows"
        systemversion "latest"

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