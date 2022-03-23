workspace "Odd"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-x64"

project "Odd"
    location "Odd"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor"
    }

    filter "system:windows"
        cppdialect "C++latest"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ODD_PLATFORM_WINDOWS",
            "ODD_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "ODD_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "ODD_RELEASE"
        optimize "On"
    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
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
        symbols "On"
    filter "configurations:Release"
        defines "ODD_RELEASE"
        optimize "On"
    filter "configurations:Distribution"
        defines "ODD_DISTRIBUTION"
        optimize "On"