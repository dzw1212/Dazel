#pragma once

#ifdef DAZEL_PLATFORM_WINDOWS
	#ifdef DAZEL_BUILD_DLL
		#define DAZEL_API __declspec(dllexport)
	#else
		#define DAZEL_API __declspec(dllimport)
	#endif
#else
	#error Not Support Platform!
#endif

#define BIT(x) (1 << x)

using UINT = unsigned int;

#ifdef DAZEL_ENABLE_ASSERTS
	#define ASSERT(condition, ...)\
	{\
		if (!(condition))\
		{\
			LOG_ERROR("Assertion Failed:{}", __VA_ARGS__);\
			__debugbreak();\
		}\
	}
	#define CORE_ASSERT(condition, ...)\
	{\
		if (!(condition))\
		{\
			CORE_LOG_ERROR("Assertion Failed:{}", __VA_ARGS__);\
			__debugbreak();\
		}\
	}
#else
	#define ASSERT(condition, ...)
	#define CORE_ASSERT(condition, ...)
#endif

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)