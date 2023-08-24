#pragma once

#include <string>
#include "glm.hpp"

namespace Odd
{
	class AudioSource
	{
	public:
		static Ref<AudioSource> Create(const std::string& file);

		AudioSource(const std::string& file);
		~AudioSource();

		const std::string& GetAudioFilePath() const { return m_AudioFilePath; }
		void Play();
		void Pause();
		void Stop();

		// Returns true if the audio source is currently playing.
		bool IsPlaying();

		bool IsLoaded() const { return m_Loaded; }
		void SetLoop(bool loop);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetPosition(glm::vec3 pos);

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;
	private:
		AudioSource() = default;

		// Loads a buffer into memory & returns the handle.
		uint32_t Load(const std::string& file);

		uint32_t m_BufferHandle = 0;
		uint32_t m_SourceHandle = 0;
		bool m_Loaded = false;
		float m_TotalDuration = 0; // in seconds

		// Attributes
		bool m_Loop = false;
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		float m_Position[3] = { 0.0f, 0.0f, 0.0f };

		std::string m_AudioFilePath;
	};
}