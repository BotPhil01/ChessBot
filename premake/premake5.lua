workspace "ChessBot"
    configurations { "Debug", "Release" }
    location ".."

project "ChessBot"
    location "build"
    kind "ConsoleApp"
    language "C++"
    targetdir "../bin/%{cfg.buildcfg}"

    files { "../src/**.hpp", "../src/**.cpp" }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
