#pragma once

#include "glm.hpp"

namespace Odd
{
	class AudioListener
	{
	public:
		// Set Master Gain
		static void SetGain(float gain);
		static void SetPosition(glm::vec3 position);
		static void SetVelocity(glm::vec3 velocity);
		static void SetOrientation(glm::vec3 at, glm::vec3 up);
	};
}