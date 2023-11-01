require "vendor/premake-ninja/ninja"

workspace "LearnOpenGL"
    location "build_lua"
    configurations { "Debug", "Release" }

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"

    -- PCH
    includedirs "src/pch"
    includedirs "include"
    pchheader "pch.h"
    pchsource "src/pch/pch.cpp"

    -- SOURCE & HEADERS
    files {
        -- REGULAR SOURCES
        "src/**.cpp",
        "src/**.h",

        -- GLAD
        "include/glad/glad.c",

        -- STB_IMAGE
        "include/stb/stb_image.cpp",

        -- IMGUI
        "include/imgui/imgui.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/imgui_impl_glfw.cpp",
        "include/imgui/imgui_impl_opengl3.cpp",

        -- FMT (Formatter)
        "lib/format.cc"
    }
    
    -- INCLUDES
    -- includedirs { "./include" }

    -- LIBS
    libdirs { "lib" }
    links {  -- TODO: some of these links are Windows-specific
        "glfw3",
        "assimp.dll",
        "libassimp-5.dll",
        "opengl32",
        "gdi32",
        "user32",
        "kernel32",
        "shell32"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        sanitize { "Address" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"