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
}