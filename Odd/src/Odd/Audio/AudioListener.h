#pragma once

#include "glm.hpp"

namespace Odd
{
	class AudioListener
	{
	public:
		~AudioListener();

		// Set Master Gain
		void SetGain(float gain);
		void SetPosition(glm::vec3 position);
		void SetVelocity(glm::vec3 velocity);
		void SetOrientation(glm::vec3 at, glm::vec3 up);
	private:
		// Master Gain
		float m_Gain = 0.0f;

		glm::vec3 m_Position{ 0.0f };
		glm::vec3 m_Velocity{ 0.0f };
		glm::vec3 m_At{ 0.0f };
		glm::vec3 m_Up{ 0.0f };
	};
}