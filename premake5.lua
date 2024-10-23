workspace "Pong"
    architecture  "x86"
    
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Pong"
    location "Source"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "LNDEBUG"
        optimize "On"