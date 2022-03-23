#pragma once

#ifdef ODD_PLATFORM_WINDOWS
	#ifdef ODD_BUILD_DLL
		#define ODD_API _declspec(dllexport)
	#else 
		#define ODD_API _declspec(dllimport)
	#endif // OD_BUILD_DLL
#else 
	#error Odd Engine Only Supports Windows
#endif

#define LEFTSHIFTBIT(x) (1 << x)