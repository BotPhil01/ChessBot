workspace "ChessBot"
    configurations { "Debug", "Release", "Tests" }
    location ".."

project "ChessBot"
    location "build"
    kind "ConsoleApp"
    language "C++"
    targetdir "../bin/%{cfg.buildcfg}"

    files { "../include/**.hpp", "../src/**.cpp" }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

    filter "configurations:Tests"
    defines { "TESTS" }
    symbols "On"
    files { "../tests/**.hpp", "../tests/**.cpp" }
    removefiles { "../src/main.cpp" }
