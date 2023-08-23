#include "Audio.h"
#include "Odd/Core/Log.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "include/AL/al.h"
#include "include/AL/alc.h"
#include "include/AL/alext.h"

namespace Odd
{
	// Physical Audio Device
	static ALCdevice* s_AudioDevice = nullptr;

	void Audio::Init()
	{
		if (InitAL(s_AudioDevice, nullptr, 0) != 0)
			DEBUG_CORE_ERROR("Audio Device Error");
	}

	void Audio::Shutdown()
	{
		CloseAL();
	}

	int Audio::InitAL(ALCdevice* device, char*** argv, int* argc)
	{
        const ALCchar* name;
        ALCcontext* ctx;

        /* Open and initialize a device */
        device = NULL;
        if (argc && argv && *argc > 1 && strcmp((*argv)[0], "-device") == 0)
        {
            device = alcOpenDevice((*argv)[1]);
            if (!device)
                fprintf(stderr, "Failed to open \"%s\", trying default\n", (*argv)[1]);
            (*argv) += 2;
            (*argc) -= 2;
        }
        if (!device)
            device = alcOpenDevice(NULL);
        if (!device)
        {
            fprintf(stderr, "Could not open a device!\n");
            return 1;
        }

        ctx = alcCreateContext(device, NULL);
        if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
        {
            if (ctx != NULL)
                alcDestroyContext(ctx);
            alcCloseDevice(device);
            fprintf(stderr, "Could not set a context!\n");
            return 1;
        }

        name = NULL;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(device) != AL_NO_ERROR)
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        printf("Opened \"%s\"\n", name);

        return 0;
	}

	void Audio::CloseAL(void)
	{
        ALCdevice* device;
        ALCcontext* ctx;

        ctx = alcGetCurrentContext();
        if (ctx == NULL)
            return;

        device = alcGetContextsDevice(ctx);

        alcMakeContextCurrent(NULL);
        alcDestroyContext(ctx);
        alcCloseDevice(device);
	}
}