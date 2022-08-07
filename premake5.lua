workspace "Odd"
    startproject "Sandbox"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-x64"

include "Odd/vendor/GLFW"
include "Odd/vendor/Glad"
include "Odd/vendor/ImGui"
include "Odd/vendor/ImGuizmo"
include "Odd/vendor/yaml-cpp"

project "Odd"
    location "Odd"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "oddpch.h"
    pchsource "Odd/src/oddpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor",
        "Odd/vendor/GLFW/include",
        "Odd/vendor/Glad/include",
        "Odd/vendor/ImGui/include",
        "Odd/vendor/yaml-cpp/include",
        "Odd/vendor/glm",
        "Odd/vendor/stb_image",
        "Odd/vendor/entt/include",
        "Odd/vendor/ImGuizmo"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "ImGuizmo",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS",
            "ODD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ODD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        runtime "Release"
        optimize "on"



project "Odd-Editor"
    location "Odd-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Odd/vendor",
        "Odd/src",
        "Odd/vendor/glm",
        "Odd/vendor/entt/include",
        "Odd/vendor/ImGuizmo"
    }

    links
    {
        "Odd"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ODD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        runtime "Release"
        optimize "on"



project "Sandbox"
    location "Examples/%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Examples/%{prj.name}/src/**.h",
        "Examples/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Odd/vendor",
        "Odd/src",
        "Odd/vendor/glm",
        "Odd/vendor/entt/include"
    }

    links
    {
        "Odd"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ODD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        runtime "Release"
        optimize "on"