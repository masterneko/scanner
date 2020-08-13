workspace "lexer"
    configurations { "Debug", "Release" }

project "lexer"
    kind "ConsoleApp"
    language "C++"
    files { "**.h", "**.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
