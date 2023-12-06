require "vendor/premake-ninja/ninja"

workspace "LearnOpenGL"
    location "build"
    configurations { "Debug", "Release" }

    buildoptions { "-fsanitize=address", "-ffile-prefix-map=..=.", "-g" }
    linkoptions { "-fsanitize=address", "-lassimp.dll", "-g" }

    filter { "system:windows" }
        postbuildcommands { "xcopy /Q /E /Y /I /D ..\\lib\\libassimp-5.dll ..\\bin\\%{cfg.buildcfg}\\" }

    filter { "not system:windows" }  -- TODO: not sure if this works
        postbuildcommands { "{COPY} ../lib/libassimp-5.dll ../bin/%{cfg.buildcfg}/" }

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}/"
    defines { "GLM_FORCE_SWIZZLE" }

    -- PCH
    includedirs "src/pch"
    includedirs "include"
    pchheader "pch.h"
    pchsource "src/pch/pch.cpp"

    dependson { "ImGUI" }

    -- SOURCE & HEADERS
    files {
        -- REGULAR SOURCES
        "src/**.cpp",
        "src/**.h",

        -- GLAD
        "include/glad/glad.h",
        "include/glad/glad.c",

        -- STB_IMAGE
        "include/stb/stb_image.h",
        "include/stb/stb_image.cpp",

        -- FMT (Formatter)
        "lib/format.cc",
    }

    -- LIBS
    libdirs { "lib" }
    links {  -- TODO: some of these links are Windows-specific
        "glfw3",
        "opengl32",
        "gdi32",
        "user32",
        "kernel32",
        "shell32",
        "ImGUI"
    }

    filter "files:**.c"
        compileas "C++"

    filter "files:**.cc"
        compileas "C++"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        -- optimize "On"
        sanitize { "Address" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter ""

project "ImGUI"
    defines { "IMGUI_IMPL_OPENGL_LOADER_CUSTOM" }

    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    -- targetdir "bin/%{cfg.buildcfg}"

    includedirs "include"
    -- PCH
    includedirs "include/glad"
    pchheader "glad.h"
    pchsource "include/glad/glad.c"

    -- SOURCE & HEADERS
    files {
        "include/imgui/*.h",
        "include/imgui/imgui.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/imgui_impl_glfw.cpp",
        "include/imgui/imgui_impl_opengl3.cpp",
    }

    -- LIBS
    copylocal "lib/libassimp5-5.dll"
    libdirs { "lib" }
    links {  -- TODO: some of these links are Windows-specific
        "assimp.dll",
        "libassimp-5.dll",
        "opengl32",
        "gdi32",
        "user32",
        "kernel32",
        "shell32",
    }

    filter "files:**.c"
        compileas "C++"

    filter "files:**.cc"
        compileas "C++"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        sanitize { "Address" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter ""