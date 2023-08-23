#pragma once

#include <string>
#include "glm.hpp"

namespace Odd
{
	class AudioSource
	{
	public:
		static AudioSource LoadFromFile(const std::string& file, bool spatial = false);

		~AudioSource();

		bool IsLoaded() const { return m_Loaded; }
		void SetPlayOnAwake(bool playOnAwake);
		void SetLoop(bool loop);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetPosition(glm::vec3 pos);
		void SetSpatial(bool spatial);

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;
	private:
		AudioSource() = default;
		AudioSource(uint32_t handle, bool loaded, float length);

		uint32_t m_BufferHandle = 0;
		uint32_t m_SourceHandle = 0;
		bool m_Loaded = false;
		float m_TotalDuration = 0; // in seconds

		// Attributes
		bool m_PlayOnAwake = true;
		bool m_Loop = false;
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		bool m_Spatial = false;

		friend class Audio;
	};
}