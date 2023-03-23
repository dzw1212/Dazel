VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {
    ["GLFW"] = "%{wks.location}/Dazel/Vender/GLFW/include",
    ["GLAD"] = "%{wks.location}/Dazel/Vender/GLAD/include",
    ["ImGui"] = "%{wks.location}/Dazel/Vender/ImGui",
    ["glm"] = "%{wks.location}/Dazel/Vender/glm",
    ["stb_image"] = "%{wks.location}/Dazel/Vender/stb_image",
    ["entt"] = "%{wks.location}/Dazel/Vender/entt/include",
    ["yaml_cpp"] = "%{wks.location}/Dazel/Vender/yaml-cpp/include",
    ["ImGuizmo"] = "%{wks.location}/Dazel/Vender/ImGuizmo",
    ["shaderc"] = "%{wks.location}/Dazel/Vender/shaderc/include",
    ["SPIRV_Cross"] = "%{wks.location}/Dazel/Vender/SPIRV_Cross",
    ["VulkanSDK"] = "%{VULKAN_SDK}/Include",
}

LibraryDir = {
    ["VulkanSDK"] = "%{VULKAN_SDK}/Lib",
    ["VulkanSDK_Debug"] = "%{wks.location}/Dazel/Vender/VulkanSDK/Lib",
}

Library = {
    ["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib",
    ["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib",

    ["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib",
    ["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib",
    ["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib",
    ["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib",

    ["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib",
    ["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib",
    ["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib",
}