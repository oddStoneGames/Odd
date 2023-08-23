#pragma once

#include <string>

class ALCdevice;
namespace Odd
{
	class Audio
	{
	public:
		// Intialize Audio Engine
		static void Init();

		// Shutdown Audio Engine
		static void Shutdown();
	private:
		static int InitAL(ALCdevice* device, char*** argv, int* argc);
		static void CloseAL(void);
	};
}