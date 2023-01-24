#include "DazelPCH.h"
#include "Log.h"

namespace DAZEL
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger = nullptr;


	void Log::Init()
	{
		s_CoreLogger = spdlog::stdout_color_mt("DazelCoreLogger");
		if (s_CoreLogger)
			s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("DazelClientLogger");
		if (s_ClientLogger)
			s_ClientLogger->set_level(spdlog::level::trace);
	}
}