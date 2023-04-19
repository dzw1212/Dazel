project "Dazel-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/DazelEditor/Resource/Script")
	objdir ("%{wks.location}/DazelEditor/Resource/Script/Intermediate")

	files 
	{
		"Src/**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Publish"
		optimize "Full"
		symbols "Off"