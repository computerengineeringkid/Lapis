workspace "Lapis"
	architecture "x64"
	startproject "Lapis"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	--Include directories relative to root folder(solution dir)
	IncludeDir = {}
	--IncludeDir["stb_image"] =	"$(SolutionDir)/Blu/engine/ExternalDependencies/stb_image"
    IncludeDir["ImGui"] = "%{wks.location}/Lapis/Engine/ExternalDependencies/imgui"
    IncludeDir["Assimp"] = "%{wks.location}/Lapis/Engine/ExternalDependencies/assimp/include"
	IncludeDir["ImGuiBackends"] = "%{wks.location}/Lapis/Engine/ExternalDependencies/imgui/backends"
    

	print("Workspace location: " .. _WORKING_DIR)

	LibraryDir = {}
    LibraryDir["Assimp"] = "%{wks.location}/Lapis/Engine/ExternalDependencies/assimp/bin/Debug-windows-x86_64/Assimp"

    Library = {}
    Library["Assimp"] = "%{LibraryDir.Assimp}/assimp.lib"

	--Windows
	Library["WinSock"] = "Ws2_32.lib"
	Library["Winmm"] = "Winmm.lib"
	Library["Version"] = "Version.lib"
	Library["Bcrypt"] = "Bcrypt.lib"
	Library["ucrt"] = "ucrt.lib"
	
	
	Library["libm"] = "libm.lib";
	Library["libcmt"] = "libcmt.lib";
	Library["libcmtd"] = "libcmtd.lib";
	Library["libucrtd"] = "libucrtd.lib"
	
	Library["ucrt"] = "ucrt.lib";
	Library["msvcrt"] = "msvcrt.lib";
	Library["msvcrtd"] = "msvcrtd.lib";


group"Dependencies"
	
	include "Lapis/Engine/ExternalDependencies/imgui"
	include "Lapis/Engine/ExternalDependencies/assimp"
	
-- Setup multiple premake files per directory so we can include them in here



	


project "Lapis"
	location "Lapis"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	characterset "MBCS"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	--pchheader "Blupch.h"
	--pchsource "Blu/engine/src/Blupch.cpp"

	files
	{
		"%{prj.name}/Engine/src/**.h",
		"%{prj.name}/Engine/src/**.cpp",
		"%{prj.name}/Engine/src/Graphics/Shaders/**.hlsl"
		
		



	}   

	includedirs 
	{
		
		"%{wks.location}/Lapis/Engine/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackends}",
		"%{IncludeDir.Assimp}",
        
	}
	
	links
	{
		
		"ImGui",
		--"%{Library.Assimp}"
		
		
		
	}
	
	flags {"NoPCH"}

	
	--buildoptions { "/wd4251" } for dll
	-- HLSL shaders
 filter "files:**.vs.hlsl"
    buildcommands {
        'fxc /T vs_5_0 /E "main" /Fo "%{wks.location}/Lapis/%{file.basename}.cso" "%{file.relpath}"'
    }
    buildoutputs {
        "%{wks.location}/Lapis/%{file.basename}.cso"
    }

filter "files:**.ps.hlsl"
    buildcommands {
        'fxc /T ps_5_0 /E "main" /Fo "%{wks.location}/Lapis/%{file.basename}.cso" "%{file.relpath}"'
    }
    buildoutputs {
        "%{wks.location}/Lapis/%{file.basename}.cso"
    }

	filter "system:windows"
		systemversion "latest"

		links 
		{
			"%{Library.WinSock}",
			"%{Library.Winmm}",
			"%{Library.Version}",
			"%{Library.Bcrypt}",
			
		}
		--"opengl32.lib"
		defines
		{
			
			"IMGUI_DEFINE_MATH_OPERATORS",
			"ASSIMP_BUILD_NO_EXPORT",   -- Ensures that we are not trying to export symbols
			"ODDL_STATIC_LIB"           -- Define
			
		}

		

	filter "configurations:Debug"
		defines "LAPIS_DEBUG"
		buildoptions "/MTd"
		symbols "on"
		--linkoptions { "/NODEFAULTLIB:MSVCRT" }

	filter "configurations:Release"
		defines "LAPIS_RELEASE"
		buildoptions "/MT"
		optimize "on"

	filter "configurations:Dist"
		defines "LAPIS_DIST"
		buildoptions "/MT"
		optimize "on"
	
		--multithreaded 
		-- refer to premake wiki on more info
	-- filter {"system:windows", "configurations:Release"}
	-- buildoptions "/MT"
