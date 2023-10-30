workspace "LearnOpenGL"
    configurations { "Debug", "Release" }

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    require "vendor/premake-ninja"
    targetdir "bin/%{cfg.buildcfg}"

    files { "**.hpp", "**.h", "**.c", "**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"