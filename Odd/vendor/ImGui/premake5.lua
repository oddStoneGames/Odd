project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/*.h",
        "include/*.cpp"
    }

    filter "system:windows"
    systemversion "latest"
    staticruntime "On"

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"
    buildoptions "/MT"

filter "configurations:Release"
    runtime "Release"
    optimize "on"
    buildoptions "/MT"