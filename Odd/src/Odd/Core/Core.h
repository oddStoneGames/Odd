#pragma once

#include <memory>

#ifdef ODD_PLATFORM_WINDOWS
#if ODD_DYNAMIC_LINK
	#ifdef ODD_BUILD_DLL
		#define ODD_API _declspec(dllexport)
	#else 
		#define ODD_API _declspec(dllimport)
	#endif // OD_BUILD_DLL
#else
	#define ODD_API
#endif
#else 
	#error Odd Engine Only Supports Windows
#endif

#define LEFTSHIFTBIT(x) (1 << x)

#define BIND_EVENT_FUNC(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Odd
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}