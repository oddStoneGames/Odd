workspace "Odd"
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

project "Odd"
    location "Odd"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "oddpch.h"
    pchsource "Odd/src/oddpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor",
        "Odd/vendor/GLFW/include",
        "Odd/vendor/Glad/include",
        "Odd/vendor/ImGui/include"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++latest"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS",
            "ODD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    filter "configurations:Release"
        defines "ODD_RELEASE"
        buildoptions "/MD"
        optimize "On"
    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        buildoptions "/MD"
        optimize "On"


project "Sandbox"
    location "Examples/%{prj.name}"
    kind "ConsoleApp"
    language "C++"

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
        "Odd/src"
    }

    links
    {
        "Odd"
    }

    filter "system:windows"
        cppdialect "C++latest"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    filter "configurations:Release"
        defines "ODD_RELEASE"
        buildoptions "/MD"
        optimize "On"
    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        buildoptions "/MD"
        optimize "On"