#pragma once

#ifdef DAZEL_PLATFORM_WINDOWS

extern DAZEL::Application* DAZEL::CreateApplication();

int main(int argc, char** argv)
{ 
	DAZEL::Log::Init();

	auto App = DAZEL::CreateApplication();
	CORE_LOG_INFO("Application start running");
	App->Run();
	delete App;
}


#endif