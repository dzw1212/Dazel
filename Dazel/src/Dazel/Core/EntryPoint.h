#pragma once

#ifdef DAZEL_PLATFORM_WINDOWS

#include "Dazel/Core/Application.h"

extern DAZEL::Application* DAZEL::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{ 
	PROFILE_BEGIN_SESSION("Startup", "DazelProfile_Startup.json");
	DAZEL::Log::Init();
	auto App = DAZEL::CreateApplication({ argc, argv });
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Startup", "DazelProfile_Runtime.json");
	CORE_LOG_INFO("Application start running");
	App->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Startup", "DazelProfile_Shutdown.json");
	delete App;
	PROFILE_END_SESSION();
}


#endif