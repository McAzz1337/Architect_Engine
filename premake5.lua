workspace "Architect_Engine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }


	outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"]		= "Architect/vendor/glfw/include"
	IncludeDir["GLEW"]		= "Architect/vendor/glew/include"
	IncludeDir["GLM"]		= "Architect/vendor/glm"
	IncludeDir["ImGui"]		= "Architect/vendor/imgui"
	IncludeDir["STB"]		= "Architect/vendor/stb"
	IncludeDir["OPENAL"]	= "Architect/vendor/openal/include"
	IncludeDir["ENTT"]		= "Architect/vendor/entt/include"


	--include "Architect/vendor/glfw"
	--include "Architect/vendor/glew"
	--include "Architect/vendor/imgui"
	--include "Architect/vendor/openal"



project "Architect"
	location "Architect"
	kind "StaticLib"
	language "C++"
	staticruntime "On"

	targetdir("bin/" .. outdir .. "/%{prj.name}")
	objdir("bin-int/" .. outdir .. "/%{prj.name}")

	-- precompiled headers
	--pchheader "t2dpc.h"
	--pchsource "T2D/t2dpc.cpp"
	
	
	files {
		"%{prj.name}/**.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",  
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.c",
		"%{prj.name}/vendor/imgui/**.h",
		"%{prj.name}/vendor/imgui/**.cpp",
		"%{prj.name}/vendor/imgui/**.inl",



		"%{prj.name}/src/**.vert",
		"%{prj.name}/src/**.geom",
		"%{prj.name}/src/**.frag"
	}			


	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}",
		"%{IncludeDir.OPENAL}",
		"%{IncludeDir.ENTT}"

	}


	libdirs {
		"Architect/vendor/glew/lib/Release/x64",
		"Architect/vendor/glfw/lib-vc2022",
		"Architect/vendor/openal/libs/Win64",
		"Architect/vendor/openal/libs/Win64/EFX-Util_MT"
	}

	links {
		"glew32s",
		"glfw3",
		"opengl32.lib",
		"OpenAl32",
		"EFX-Util"
	}


	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines {
			 "ARCHT_PLATFORM_WINDOWS",
			 "GLEW_STATIC",
			 "IMGUI_IMPL_OPENGL_LOADER_GLEW",
			 "GLM_FORCE_RADIANS",
			 "_CRT_SECURE_NO_WARNINGS",
			 "_USE_MATH_DEFINES"
		}

	

		filter "configurations:Debug"
			defines "ARCHT_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "ARCHT_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "ARCHT_DIST"
			buildoptions "/MD"
			optimize "On"


project "PoKeMeMe"
	location "PoKeMeMe"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outdir .. "/%{prj.name}")
	objdir("bin-int/" .. outdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",  
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.vert",
		"%{prj.name}/src/**.geom",
		"%{prj.name}/src/**.frag"
	}

	includedirs {
		"Architect",
		"Architect/src",
		"Architect/vendor",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.STB}",
		"%{IncludeDir.OPENAL}",
		"%{IncludeDir.ENTT}"
	}

	libdirs {
		"Architect/vendor/glew/lib/Release/x64",
		"Architect/vendor/glfw/lib-vc2015",
		"Architect/vendor/openal/libs/Win64",
		"Architect/vendor/openal/libs/Win64/EFX-Util_MT"
	}

	links {
		"Architect"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines {
			 "ARCHT_PLATFORM_WINDOWS",
			 "GLEW_STATIC",
			 "IMGUI_IMPL_OPENGL_LOADER_GLEW",
			 "GLM_FORCE_RADIANS",
			 "_CRT_SECURE_NO_WARNINGS",
			 "_USE_MATH_DEFINES"
		}


		filter "configurations:Debug"
			defines "ARCHT_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "ARCHT_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "ARCHT_DIST"
			buildoptions "/MD"
			optimize "On"











	