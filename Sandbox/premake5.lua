project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/Dazel/src",
        "%{wks.location}/Dazel/Vender/spdlog/include",
        "%{wks.location}/Dazel/Vender",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
    }

    links "Dazel"

    filter "system:windows"
        systemversion "latest"

        -- defines
        -- {
        --     "DAZEL_PLATFORM_WINDOWS",
        --     "DAZEL_ENABLE_ASSERTS",
        --     "_CRT_SECURE_NOT_WARNINGS",
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
