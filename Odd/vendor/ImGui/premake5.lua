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

    defines { "IMGUI_API=__declspec( dllexport )" }

    filter "system:windows"
    systemversion "latest"
    staticruntime "On"

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"