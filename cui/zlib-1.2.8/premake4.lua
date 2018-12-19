-- A solution contains projects, and defines the available configurations
solution "zlib"
configurations { "Debug", "Release" }

-- A project defines one build target
project "zlib"
--kind "SharedLib"
kind "StaticLib"
language "C"
files { "*.h", "*.c" }
includedirs { ".", }

configuration "Debug"
defines { "DEBUG" }
flags { "Symbols" }
targetdir "debug"

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize" }  
targetdir "release"
