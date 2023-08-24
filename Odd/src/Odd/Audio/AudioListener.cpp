#include "AudioListener.h"

#include "include/AL/al.h"

namespace Odd
{
	void AudioListener::SetGain(float gain)
	{
		alListenerf(AL_GAIN, gain);
	}

	void AudioListener::SetPosition(glm::vec3 position)
	{
		ALfloat listenerPos[] = { position.x, position.y, position.z };
		alListenerfv(AL_POSITION, listenerPos);
	}

	void AudioListener::SetVelocity(glm::vec3 velocity)
	{
		ALfloat listenerVel[] = { velocity.x, velocity.y, velocity.z };
		alListenerfv(AL_VELOCITY, listenerVel);
	}

	void AudioListener::SetOrientation(glm::vec3 at, glm::vec3 up)
	{
		ALfloat listenerOri[] = { at.x, at.y, at.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, listenerOri);
	}
}