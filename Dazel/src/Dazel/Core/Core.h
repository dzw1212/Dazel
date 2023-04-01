#pragma once

#include <memory>

#ifdef DAZEL_PLATFORM_WINDOWS
	#ifdef DAZEL_DYNAMIC_LINK
		#ifdef DAZEL_BUILD_DLL
			#define DAZEL_API __declspec(dllexport)
		#else
			#define DAZEL_API __declspec(dllimport)
		#endif
	#else
		#define DAZEL_API
	#endif
#else
	#error Not Support Platform!
#endif

#define BIT(x) (1 << x)

using UINT = uint32_t;
using UINT64 = uint64_t;

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

namespace DAZEL
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
